import argparse
import Obstacles
import numpy as np
import matplotlib.pyplot as plt
import configparser
from matplotlib import ticker

'''

python3 shortest_path.py --genObst --obstacleCount 1000 -o ../examples/obstacles50.txt -c ../examples/input50.par

../build/src/shortest_path -c input50.par

python3 shortest_path.py --plot ../examples/shortest_path.coords --obstacles ../examples/obstacles50.txt -c ../examples/input50.par -o shortest_path.png 

'''

def main():
    parser = argparse.ArgumentParser(description='A generic Python script with argument parsing.')
    parser.add_argument('-o', '--outfile', type=str, default='', help='Output file name')
    parser.add_argument('--plotPath', type=str, default='', help='Plot shortest path solution')
    parser.add_argument('--obstacles', dest='obstaclesFile', type=str, default='', help='File with obstacle coordinates')
    parser.add_argument('--genObstacles', action='store_true', help='Generate obstacles')
    parser.add_argument('--obstacleCount', type=int, required=False, default=10, help='Number of obstacles to generate')
    parser.add_argument('-c', '--config', dest='configFile', type=str, default='', help='Configuration file')
        
    args = parser.parse_args()

    config = None
    if args.configFile != '':
        config = configparser.ConfigParser()
        with open(args.configFile) as stream:
            config.read_string("[top]\n" + stream.read())
        config = config['top']
        
    if args.genObstacles:
        print("Generating obstacles")
        xmin, xmax = float(config['field_xmin']), float(config['field_xmax'])
        ymin, ymax = float(config['field_ymin']), float(config['field_ymax'])
        xi, yi = float(config['x']), float(config['y'])
        xf, yf = float(config['xf']), float(config['yf'])
        excludedCoords = [[xi, yi], [xf, yf]]
        Obstacles.Obstacles(xmin, xmax, ymin, ymax, args.obstacleCount, excludedCoords).save_obstacles(args.outfile)

    obstacles = None
    if args.obstaclesFile != '':
        print("Loading obstacles")
        obstacles = np.loadtxt(args.obstaclesFile)

    if args.plotPath != '':
        print("Plotting path")
        path = np.loadtxt(args.plotPath)
        plot_path(obstacles, path, config, args.outfile)
    
def plot_path(obstacles, path, config, outfile):
    fig = plt.figure(figsize=(8, 8))
    for x,y in obstacles:
        square = plt.Rectangle((x, y), 1, 1, fc='red', edgecolor='red', linewidth=1)
        plt.gca().add_patch(square)
    # plt.scatter(obstacles[:, 0], obstacles[:, 1], color='red', marker='s', s=.5)
    plt.plot(path[:, 0] + 0.5, path[:, 1] + 0.5, color='g', marker='o', markersize=5, lw=2)
    plt.gca().set_aspect('equal', adjustable='box')
    plt.grid(True)
    # set ticks
    if config is not None:
        xmin, xmax = float(config['field_xmin']), float(config['field_xmax'])
        ymin, ymax = float(config['field_ymin']), float(config['field_ymax'])
        # plt.xticks(np.arange(xmin + 0.5, xmax + 1, 1))
        # plt.yticks(np.arange(ymin + 0.5, ymax + 1, 1))
        plt.xticks(np.arange(xmin, xmax + 0.5, 1))
        plt.yticks(np.arange(ymin, ymax + 0.5, 1))
        field = plt.Rectangle((xmin, ymin), xmax - xmin + 1, ymax - ymin + 1, fill=False, edgecolor='k', linewidth=5)
        plt.gca().add_patch(field)

        # plot destination point
        xf, yf = float(config['xf']), float(config['yf'])
        plt.plot([xf + 0.5], [yf + 0.5], color='k', marker='x', markersize=5, lw=2)

    ax = plt.gca()
    ax.set_xticklabels([])
    ax.set_yticklabels([])

    ax.set_xlim(xmin, xmax + 1)
    ax.set_ylim(ymin, ymax + 1)
    
    plt.subplots_adjust(left=0.01, right=0.99, top=0.99, bottom=0.01)
    
    if outfile != '':
        plt.savefig(outfile, format='png', dpi=200)
    else:
        plt.show()

if __name__ == "__main__":
    main()