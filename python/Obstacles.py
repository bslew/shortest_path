import random
import numpy as np

class Obstacles:
    def __init__(self, xmin, xmax, ymin, ymax, num_obstacles, excluded_coords):
        self.nX = xmax - xmin
        self.nY = ymax - ymin
        self.xmin = xmin
        self.ymin = ymin
        self.xmax = xmax
        self.ymax = ymax
        self.num_obstacles = num_obstacles
        self.excluded_coords = excluded_coords
        self.obstacles = self.generate_obstacles()

    def generate_obstacles(self):
        obstacles = []
        while len(obstacles) < self.num_obstacles:
            x = random.randint(0, self.nX) + self.xmin
            y = random.randint(0, self.nY) + self.ymin
            if [x, y] not in self.excluded_coords:
                obstacles.append([x, y])
        return np.asarray(obstacles)

    def get_obstacles(self):
        return self.obstacles
    
    def save_obstacles(self, filename):
        np.savetxt(filename, self.obstacles, fmt='%d', delimiter=' ')