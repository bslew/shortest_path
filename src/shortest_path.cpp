/* This program implements calculation of finding shortest path from start node
 * to end node of any graph.
 *
 * Jan 1, 2022, 8:25:00 PM
 */

/*!
 * \file test.cpp
 *
 *  Project: shortest_path
 *  Created on: Jan 1, 2022 8:26:08 PM
 *  Author: blew
 */

#include "Graph.h"
#include "Obstacles.h"
#include "Path.h"
#include <assert.h>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <iostream>
#include <math.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <time.h>

//#include <yaml-cpp/yaml.h>

#define PARAMETER_FILE "input.par"

using namespace std;

spdlog::logger getLogger(int verbosity);
string getCmdString(int argc, char** argv);
string getProgramVersionString();
boost::program_options::variables_map parseOptions(int argc, char** argv);

int main(int argc, char** argv) {

    // initialize parser
    boost::program_options::variables_map opt = parseOptions(argc, argv);

    spdlog::logger logger = getLogger(opt["verbosity"].as<int>());
    logger.info("shortest_path - NEW RUN");
    logger.info(getCmdString(argc, argv));

    //	std::cout << opt["input_file"].as< vector<string> >() << std::endl;
    //	if (opt.count("input-file")) {
    //	vector<string> input_files=opt["input-file"].as< vector<string> >();

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

    //	exit(0);
    //	std::cout << "x: " << opt["x"].as<int>() << "\n";

    ///
    /// Loading obstacles as list of i,j coordinates in the field
    ///

    logger.info("Loading obstacles from {}", opt["obst"].as<string>());
    minpath::Obstacles<int> field;
    field.load(opt["obst"].as<string>());
    logger.info("Loaded {} obstacle points", field.size());

    long imin = opt["field_xmin"].as<int>();
    long imax = opt["field_xmax"].as<int>();
    long jmin = opt["field_ymin"].as<int>();
    long jmax = opt["field_ymax"].as<int>();
    long ist = opt["x"].as<int>();
    long jst = opt["y"].as<int>();
    long ien = opt["xf"].as<int>();
    long jen = opt["yf"].as<int>();

    //
    // build a graph
    //
    logger.info("Building graph");
    minpath::FieldGraph<int> G(ist, jst, ien, jen, imin, imax, jmin, jmax,
                               field);

    // G.node(0).neighbors()[3] = 2;

    logger.info("Graph done");

    if (opt["pg"].as<bool>()) {
        std::cout << "Graph\n" << G << "\n";
        exit(0);
    }

    if (opt["dg"].as<string>() != "") {
        G.dumpGraph(opt["dg"].as<string>());
    }

    if (opt["algo"].as<string>() == "MinimalDistance") {
        auto minDist = G.solveMinimalDistance();
        if (minDist.has_value()) {
            logger.info("Minimal distance to destination node is {0}",
                        minDist.value());
        } else {
            logger.info("End node cannot be reached");
        }
        return 0;
        // } else if (opt["algo"].as<string>() == "MinimalStepsCount") {
        //     auto minDist = G.solveMinimalStepsCount();

        //     std::cout << "minimal distance to destination node is " <<
        //     minDist
        //               << std::endl;
        //     return 0;
    }

    // minpath::Path p(opt["x"].as<int>(), opt["y"].as<int>(),
    // opt["xf"].as<int>(),
    //                 opt["yf"].as<int>(), opt["verbosity"].as<int>());

    // logger.info("Starting walk");
    // p.step(opt["x"].as<long>(), opt["y"].as<long>());

    return 0;

    //
    // iterate over all input files in parallel
    //
    //	vector<string>::const_iterator fIt;
    long i = 0;
    //#pragma omp parallel for private(i) firstprivate(channels)
    //	for (auto infile : input_files) {
    //	for (fIt=input_files.begin(); fIt!=input_files.end();++fIt) {
    //	for (i=0;i<input_files.size();i++) {
    //		string infile=*fIt;
    //		string infile=input_files[i];
    //    	logger.info("Processing file: {}", infile);
    //    	logger.debug("loading data...");

    //    	DirectionAh
    //    ahSunCmd(opt["sunA"].as<double>(),opt["sunh"].as<double>());
    //	}

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
            // po::bool_switch()->default_value(false), "print program version
            // and exit")
            //		    ("switch,s",
            // po::bool_switch()->default_value(false), "switch option")
            ("help", "produce help message")(
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
            ("obst", po::value<string>()->default_value(""),
             "Obstacles file (space separated x,y integer coordinates.")(
                "pg", po::value<bool>()->default_value(false),
                "print graph and exit")
            //		    ("mask",
            // po::value<string>(&stropt)->default_value(""), "specify mask as
            // one of: \n" 					"c,A,Z,r -- eg.
            // c,-50,50,0.5
            // - will create a circular mask on sphere in deg."
            // "Azimuth from south westwards")
            ("dg", po::value<string>()->default_value(""), "dump graph")(
                "algo", po::value<string>()->default_value("dijkstra"),
                "select algorithm."
                "For now only dijkstra is possible")

                ("x", po::value<int>()->default_value(0),
                 "x start")("y", po::value<int>()->default_value(0), "y start")(
                    "xf", po::value<int>()->default_value(0),
                    "x end")("yf", po::value<int>()->default_value(0), "y end")(
                    "field_xmin", po::value<int>()->default_value(0),
                    "minimal x coordinate of the field")(
                    "field_ymin", po::value<int>()->default_value(0),
                    "minimal y coordinate of the field")(
                    "field_xmax", po::value<int>()->default_value(0),
                    "maximal x coordinate of the field")(
                    "field_ymax", po::value<int>()->default_value(0),
                    "maximal y coordinate of the field")

            ;

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
            cerr << "cannot open config file: " << config_file
                 << " (ignoring)\n";
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
            cout << "Using parameter file " << parameter_file << "\n";
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
        }
        /*
                else {
                        cout << "No input files provided" << std::endl;
                        exit(0);
                }
        */

        if (verbo > 0)
            cout << "Verbosity level is " << verbo << "\n";
    } catch (exception& e) {
        cout << e.what() << "\n";
        exit(1);
    }

    return vm;
}
/* ********************************************************************************************
 */
string getProgramVersionString() {
    string rev;
    rev = "0.1.1";

#ifdef GOMPI
    rev += " (MPI)";
#endif

    return rev;
}
/* ********************************************************************************************
 */
spdlog::logger getLogger(int verbosity) {
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_sink->set_level(spdlog::level::info);
    //    console_sink->set_level(spdlog::level::debug);
    //    console_sink->set_pattern("[test] [%^%l%$] %v");

    auto file_sink =
        std::make_shared<spdlog::sinks::basic_file_sink_mt>("test.log", true);
    file_sink->set_level(spdlog::level::debug);

    spdlog::logger logger("test", {console_sink, file_sink});
    logger.set_level(spdlog::level::info);

    if (verbosity > 2) {
        //		cout << "setting verbosity " <<
        // opt["verbosity"].as<int>()
        //<< "\n";
        logger.sinks()[0]->set_level(spdlog::level::debug);
        logger.sinks()[1]->set_level(spdlog::level::debug);
        logger.set_level(spdlog::level::debug);
    }

    return logger;
}
/* ********************************************************************************************
 */
string getCmdString(int argc, char** argv) {
    stringstream ss;
    for (long i = 0; i < argc; i++) {
        ss << string(argv[i]) << " ";
    }
    return ss.str();
}
/* ********************************************************************************************
 */
