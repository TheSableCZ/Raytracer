
import subprocess
import numpy as np
import matplotlib.pyplot as plt

exe_path = "cmake-build-release/Raytracer"
scenes = [5, 6]
scene_names = {
    5: "Cruiser",
    6: "Bunny"
}
acceleration_structures = [2, 3]
ads_names = {
    2: "Octree",
    3: "AABBVH"
}
no_samples = 20

#levels_to_test = range(4, 11)
levels_to_test = range(4, 17)

for scene in scenes:

    x = np.arange(len(levels_to_test))
    width = 0.35
    fig, ax = plt.subplots()

    cnt = 0
    for ads in acceleration_structures:
        res = []
        for level in levels_to_test:
            subprocess.run([exe_path, str(scene), str(ads), str(no_samples), str(level)])

            f = open("stats.txt")
            res.append(float(f.read().split("\n")[0]))
            f.close()

        offset = cnt * width
        rects1 = ax.bar(x - width/2 + offset, res, width, label=ads_names[ads]) # label='Avg sample time - ' + str(no_samples) + " samples")
        cnt += 1

    ax.set_xlabel("Maximum tree depth")
    ax.set_ylabel('Avg time to render 1 sample (Seconds)')
    ax.set_xticklabels([str(i) for i in levels_to_test])
    ax.set_title("Scene: " + scene_names[scene] + ', avg sample time - ' + str(no_samples) + " samples")
    ax.legend()
    plt.savefig("avg_time_s" + str(scene) + ".png") #+ "_a" + str(ads) +

