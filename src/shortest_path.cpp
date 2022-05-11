/* This program implements calculation of finding shortest path from start node
 * to end node on any graph.
 *
 * Jan 1, 2022, 8:25:00 PM
 */

#include "Graph.h"
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

using namespace std;

string getCmdString(int argc, char** argv);
string getProgramVersionString();
boost::program_options::variables_map parseOptions(int argc, char** argv);

int main(int argc, char** argv) {

    // initialize parser
    boost::program_options::variables_map opt = parseOptions(argc, argv);

    // initialize looger
    auto logger = minpath::getLogger(opt["verbosity"].as<int>());
    logger.debug("shortest_path - NEW RUN");
    logger.debug(getCmdString(argc, argv));

    // Load obstacles as list of i,j coordinates in the field
    logger.debug("Loading obstacles from {}", opt["obst"].as<string>());
    minpath::Obstacles<int> field;
    field.load(opt["obst"].as<string>());
    logger.debug("Loaded {} obstacle points", field.size());

    // define rectangular grid parameters
    long imin = opt["field_xmin"].as<int>();
    long imax = opt["field_xmax"].as<int>();
    long jmin = opt["field_ymin"].as<int>();
    long jmax = opt["field_ymax"].as<int>();
    bool diagonal = opt["field_diag_connections"].as<bool>();
    // define inital and final coordinates
    long ist = opt["x"].as<int>();
    long jst = opt["y"].as<int>();
    long ien = opt["xf"].as<int>();
    long jen = opt["yf"].as<int>();

    // build a graph
    logger.debug("Building graph");
    minpath::FieldGraph<int> G(ist, jst, ien, jen, imin, imax, jmin, jmax,
                               field, diagonal, logger);

    if (opt["pg"].as<bool>()) {
        logger.debug("Printing graph");
        std::cout << "Graph\n" << G << "\n";
    }

    if (opt["dg"].as<string>() != "") {
        logger.debug("Dumping graph to file: {0}", opt["dg"].as<string>());
        G.dumpGraph(opt["dg"].as<string>());
    }

    if (opt["algo"].as<string>() == "MinimalDistance") {
        auto minDist = G.solveMinimalDistance();
        if (minDist.has_value()) {
            auto shortestPath = G.getShortestPathNodes();
            if (logger.should_log(spdlog::level::level_enum::info)) {
                minpath::printVec<size_t>(shortestPath, "min path nodes");
            }
            logger.info("Minimal distance to destination node is {0}",
                        minDist.value());
            if (opt["dsp"].as<string>() != "") {
                G.dumpShortestPath(opt["dsp"].as<string>());
            }
        } else {
            logger.info("End node cannot be reached");
        }
        return 0;
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
        string stropt;
        vector<string> svec;
        stringstream ss;
        ss << std::getenv("HOME") << "/.config/shortest_path/config.txt";
        string config_file = ss.str();
        boost::filesystem::path cwd = boost::filesystem::current_path();
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
                             po::value<int>(&verbo)->default_value(5),
                             "verbosity level")
            //            ("include-path,I",
            //                 po::value< vector<string> >()->composing(),
            //                 "include path")
            ("obst", po::value<string>()->default_value(""),
             "Obstacles file (space separated x,y integer coordinates.")(
                "pg", po::value<bool>()->default_value(false),
                "print graph and exit")(
                "dg", po::value<string>()->default_value(""),
                "dump graph")("dsp", po::value<string>()->default_value(""),
                              "dump shortest path")(
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
                    "maximal y coordinate of the field")(
                    "field_diag_connections",
                    po::value<bool>()->default_value(false),
                    "connect nodes on rectangular grid along diagonals of "
                    "neighboring nodes forming a square")

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
        } else {
            store(parse_config_file(ifs, config_file_options), vm);
            notify(vm);
        }

        // process parameter file
        ifs.open(parameter_file.c_str());
        if (!ifs.is_open()) {
            cerr << "cannot open parameter file: " << parameter_file << "\n";
        } else {
            // cout << "Using parameter file " << parameter_file << "\n";
            store(parse_config_file(ifs, parameter_file_options), vm);
            notify(vm);
        }

        if (vm.count("help")) {
            cout << visible << "\n";
            exit(0);
        }

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
