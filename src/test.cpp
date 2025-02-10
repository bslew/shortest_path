/*!
 * \file test.cpp
 *
 *  Project: shortest_path
 *  Created on: Jan 1, 2022 8:26:08 PM
 *  Author: blew
 */

#include "Logger.h"
#include <assert.h>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <time.h>

//#include <yaml-cpp/yaml.h>

#define PARAMETER_FILE "input.par"

using namespace std;

string getCmdString(int argc, char** argv);
string getProgramVersionString();
boost::program_options::variables_map parseOptions(int argc, char** argv);

int main(int argc, char** argv) {

    // initialize parser
    boost::program_options::variables_map opt = parseOptions(argc, argv);

    spdlog::logger logger = minpath::getLogger(opt["verbosity"].as<int>());
    logger.info("test - NEW RUN");
    logger.info(getCmdString(argc, argv));

    //	std::cout << opt["input_file"].as< vector<string> >() << std::endl;
    //	if (opt.count("input-file")) {
    vector<string> input_files = opt["input-file"].as<vector<string>>();

    //
    // save yaml config file for the dataset
    //
    //	YAML::Node config;
    //	config["orig_ns"]=opt["ns"].as<long>();
    //	config["reg_ns"]=opt["nsc"].as<long>();
    //	config["hmin"]=opt["hmin"].as<double>();
    //	for (auto ch : channels) {
    //		config["channels"].push_back(ch);
    //	}
    //	std::ofstream fout(opt["odir"].as<string>()+"/config.yaml");
    //	fout << config;
    //	fout.close();

    //
    // iterate over all input files in parallel
    //
    //	vector<string>::const_iterator fIt;
    long i = 0;
    //#pragma omp parallel for private(i) firstprivate(channels)
    //	for (auto infile : input_files) {
    //	for (fIt=input_files.begin(); fIt!=input_files.end();++fIt) {
    for (i = 0; i < input_files.size(); i++) {
        //		string infile=*fIt;
        string infile = input_files[i];
        logger.info("Processing file: {}", infile);
        logger.debug("loading data...");

        //    	DirectionAh
        //    ahSunCmd(opt["sunA"].as<double>(),opt["sunh"].as<double>());
    }

    return 0;
}

/* ********************************************************************************************
 */
boost::program_options::variables_map parseOptions(int argc, char** argv) {
    namespace po = boost::program_options;
    po::variables_map vm;

    try {
        int verbo = 1;
        bool boolopt;
        long longopt;
        double dbl;
        string stropt;
        vector<string> svec;
        stringstream ss;
        ss << std::getenv("HOME") << "/.config/shortest_path/config.txt";
        string config_file = ss.str();
        boost::filesystem::path cwd = boost::filesystem::current_path();
        //        string parameter_file=cwd.string()+"/input.par";
        string parameter_file = "input.par";

        // Declare a group of options that will be
        // allowed only on command line
        po::options_description generic("Generic options");
        generic.add_options()("version,V", "print version string")
            //			("ifile,i",
            // po::value<string>(&stropt)->multitoken()->required(), "input file
            // name")
            //		    ("version,V",
            //po::bool_switch()->default_value(false), "print program version
            //and exit")
            ("switch,s", po::bool_switch()->default_value(false),
             "switch option")("help", "produce help message")(
                "config,c",
                po::value<string>(&config_file)->default_value(config_file),
                "name of a global ACClass configuration file.")(
                "param,p",
                po::value<string>(&parameter_file)
                    ->default_value(parameter_file),
                "parameter file");

        // Declare a group of options that will be
        // allowed both on command line and in
        // config file
        po::options_description config("Configuration");
        config.add_options()("verbosity,v",
                             po::value<int>(&verbo)->default_value(0),
                             "verbosity level")
            //            ("include-path,I",
            //                 po::value< vector<string> >()->composing(),
            //                 "include path")
            //			("ifile,i",
            //po::value<string>(&stropt)->default_value(""), "Input file name.")
            ("odir", po::value<string>(&stropt)->default_value(""),
             "output file name")("mask",
                                 po::value<string>(&stropt)->default_value(""),
                                 "specify mask as one of: \n"
                                 "c,A,Z,r -- eg. c,-50,50,0.5 - will create a "
                                 "circular mask on sphere in deg."
                                 "Azimuth from south westwards")(
                "show", po::value<bool>(&boolopt)->default_value(false),
                "shows the loaded image")

                ("hmin", po::value<double>(&dbl)->default_value(0),
                 "Minimal elevation threshold.");

        // Hidden options, will be allowed both on command line and
        // in config file, but will not be shown to the user.
        po::options_description hidden("Hidden options");
        hidden.add_options()("input-file", po::value<vector<string>>(),
                             "input files");

        po::options_description cmdline_options;
        cmdline_options.add(generic).add(config).add(hidden);

        po::options_description config_file_options;
        config_file_options.add(config).add(hidden);

        po::options_description parameter_file_options;
        parameter_file_options.add(config).add(hidden);

        po::options_description visible(
            "test\n\n "
            "This program ... "
            "\n\n. "
            "\n"
            "example usage: ${file_base} [input_files]"
            "\n"
            "\n\n"
            "Allowed options");
        visible.add(generic).add(config);

        po::positional_options_description p;
        p.add("input-file", -1);

        store(po::command_line_parser(argc, argv)
                  .options(cmdline_options)
                  .positional(p)
                  .run(),
              vm);
        notify(vm);

        // process config file
        ifstream ifs(config_file.c_str());
        if (!ifs) {
            cerr << "cannot open config file: " << config_file << "\n";
            //            return 0;
        } else {
            store(parse_config_file(ifs, config_file_options), vm);
            notify(vm);
        }

        // process parameter file
        ifs.open(parameter_file.c_str());
        if (!ifs.is_open()) {
            cerr << "cannot open parameter file: " << parameter_file << "\n";
            //            return 0;
        } else {
            store(parse_config_file(ifs, parameter_file_options), vm);
            notify(vm);
        }

        if (vm.count("help")) {
            cout << visible << "\n";
            exit(0);
        }

        //    	if (vm["version"].as<bool>()) {
        //    		std::cout << getProgramVersionString() << std::endl;
        //    		exit(0);
        //    	}

        if (vm.count("version")) {
            std::cout << getProgramVersionString() << std::endl;
            exit(0);
        }

        if (vm.count("input-file")) {
            if (verbo > 0) {
                cout << "Input files are: \n";
                for (auto& s : vm["input-file"].as<vector<string>>()) {
                    cout << s << " ";
                }
                cout << "\n";
            }
        } else {
            cout << "No input files provided" << std::endl;
            exit(0);
        }

        if (verbo > 0)
            cout << "Verbosity level is " << verbo << "\n";
    } catch (exception& e) {
        cout << e.what() << "\n";
        exit(1);
    }

    return vm;
}

string getProgramVersionString() {
    string rev;
    rev = "0.1.1";

#ifdef GOMPI
    rev += " (MPI)";
#endif

    return rev;
}

string getCmdString(int argc, char** argv) {
    stringstream ss;
    for (long i = 0; i < argc; i++) {
        ss << string(argv[i]) << " ";
    }
    return ss.str();
}
