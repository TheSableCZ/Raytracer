
import subprocess
import numpy as np
import matplotlib.pyplot as plt

exe_path = "cmake-build-release/Raytracer"
scenes = [5, 6]
acceleration_structures = [2]
no_samples = 20

levels_to_test = range(4, 11)

for scene in scenes:
    for ads in acceleration_structures:
        res = []
        for level in levels_to_test:
            subprocess.run([exe_path, str(scene), str(ads), str(no_samples), str(level)])

            f = open("stats.txt")
            res.append(float(f.read().split("\n")[0]))
            f.close()

        x = np.arange(len(levels_to_test))
        width = 0.35
        fig, ax = plt.subplots()
        rects1 = ax.bar(x, res, width, label='Avg sample time - ' + str(no_samples) + " samples")
        ax.set_ylabel('Seconds')
        ax.set_xticklabels(["", *levels_to_test])
        ax.legend()
        plt.savefig("avg_time_s" + str(scene) + "_a" + str(ads) + ".png")

