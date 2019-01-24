/*-
 * Copyright (C) 2018, Shanghai Trusted Industry Control Platform Co., Ltd.
 * Contributed by  (Minghui Zheng) <zhengminghui@ticpsh.com> 
 */

#include <iostream>
#include <cstdio>
#include <fstream>
#include <string>
#include <regex>
#include <limits.h>
#include <vector>
#include <boost/filesystem.hpp>

struct CompileMessage
{
    std::string filepath;
    std::string errtype;
    std::string errposition;
    std::string errcode;
    std::string errmessage;
};

/**
 * @brief return the clang compile message store in a vector.
 * @param project_dir: project directory path.
 * @param file: name or the relative path to the \p project_dir of the source file.
 *
 * Example:
 *  vector<CompileMessage> test;
 *  bool passed;
 *  std::tie(passed, test) = GetCompileMessage("", argv[1]);
 */
std::pair<bool, std::vector<CompileMessage>> GetCompileMessage(const boost::filesystem::path &project_dir, const std::string &file, const std::string &include_option = "", const std::string &define_option = "");

