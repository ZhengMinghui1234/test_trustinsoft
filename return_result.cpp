/*-
 * Copyright (C) 2018, Shanghai Trusted Industry Control Platform Co., Ltd.
 * Contributed by  (Minghui Zheng) <zhengminghui@ticpsh.com>
 */

#include <boost/iostreams/device/file_descriptor.hpp>
#include <boost/iostreams/stream.hpp>
#include "return_result.h"

namespace fs = boost::filesystem;
static const char *compile_command = "./cc.sh";

std::pair<bool, std::vector<CompileMessage>> GetCompileMessage(const fs::path &project_dir, const std::string &file, const std::string &include_option, const std::string &define_option)
{
  bool return_value;
  std::vector<std::string> err_all;
  std::vector<CompileMessage> msg;
  struct CompileMessage Output;
  if (!fs::exists(project_dir / file)) {
    Output.filepath = file;
    Output.errtype = "-1";
    Output.errposition = "0";
    Output.errcode = "-1";
    Output.errmessage = "The file doesn't exit !";
    msg.push_back(Output);
    return std::make_pair(false, std::move(msg));
  }
  std::string command = compile_command;
  if (!project_dir.empty()) {
    command += " -H " + project_dir.string();
  }
  if (!include_option.empty()) {
    command += " -I " + include_option;
  }
  if (!define_option.empty()) {
    command += " -D " + define_option;
  }
  command += " -F " + file;
  command += " 2>&1";
  FILE *errorfile = popen(command.c_str(), "r");
  if (errorfile) {
    boost::iostreams::stream<boost::iostreams::file_descriptor_source> error_stream(boost::iostreams::file_descriptor_source(fileno(errorfile), boost::iostreams::never_close_handle));
    std::string temp_err;
    while (getline(error_stream, temp_err)) {
      err_all.push_back(std::move(temp_err));
    }
    error_stream.close();
    return_value = (pclose(errorfile) == 0);
  } else {
    throw std::runtime_error(std::string("Failed to launch compile module") + strerror(errno));
  }
  int vec_len = err_all.size();
  int i = 0;
  while (i < vec_len)
  {
    std::string tmp = err_all[i];
    int length = tmp.length();
    if (regex_match(tmp, std::regex("(.*)(clang)(.*)"))) {
      Output.filepath = file;
      Output.errtype = "-1";
      Output.errposition = "0";
      Output.errcode = "-1";
      Output.errmessage = tmp;
      msg.push_back(Output);
      return_value = false;
      break;
    }
    if (regex_match(tmp, std::regex("(.*)(fatal error:)(.*)"))) {
      int posi1 = tmp.find(" ");
      int posi2 = tmp.find(" ", posi1 + 1);
      int posi3 = tmp.find(" ", posi2 + 1);
      std::string temp_val;
      if (i+1 < vec_len) {
        temp_val = err_all[i + 1];
      }
      else {
        temp_val = "-1";
      }
      Output.filepath = file;
      Output.errtype = tmp.substr(posi1 + 1, posi3 - posi1 - 2);
      Output.errposition = tmp.substr(0, posi1 - 1);
      Output.errcode = temp_val;
      Output.errmessage = tmp.substr(posi2 + 1, length - posi2);
      msg.push_back(Output);
      return_value = false;
      break;
    }
    if (regex_match(tmp, std::regex("(.*)(error:)(.*)")) || regex_match(tmp, std::regex("(.*)(warning:)(.*)")))
    {
      int posi1 = tmp.find(" ");
      int posi2 = tmp.find(" ", posi1 + 1);
      std::string temp_val;
      if (i + 1 < vec_len) {
        temp_val = err_all[i + 1];
      }
      else {
        temp_val = "-1";
      }
      Output.filepath = file;
      Output.errtype = tmp.substr(posi1 + 1, posi2 - posi1 - 2);
      Output.errposition = tmp.substr(0, posi1 - 1);
      Output.errcode = temp_val;
      Output.errmessage = tmp.substr(posi2 + 1, length - posi2);
      msg.push_back(Output);
      if (i + 2 < vec_len) {
        i = i + 2;
      } 
    }
    ++i;
  }
  if (!return_value && msg.empty()) {
    std::stringstream ss;
    for (auto err : err_all) {
      ss << err << std::endl;
    }
    throw std::runtime_error("Compiling return false with output: " + ss.str());
  }
  return std::make_pair(return_value, std::move(msg));
}

