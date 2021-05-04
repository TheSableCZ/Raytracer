import os
import signal
import subprocess
import time

import numpy as np
import matplotlib.pyplot as plt
from threading import Timer

exe_path = "cmake-build-release/Raytracer"
from_file = False
scenes = [8]
scene_names = {
    5: "Cruiser",
    6: "Bunny",
    7: "Dyno",
    8: "Room"
}
acceleration_structures = [2]
ads_names = {
    2: "Octree",
    3: "AABBVH"
}
no_samples = 20
timeout = 300
wait_timeout = 40

#levels_to_test = range(4, 11)
levels_to_test = range(14, 17)

#kill = lambda process: os.kill(process.pid, signal.SIGINT)
#kill = lambda process: process.kill()


def kill(process):
    os.kill(process.pid, signal.SIGINT)
    time.sleep(wait_timeout)
    process.kill()


def write_list(_list, filename):
    with open(filename, "w") as file:
        for listitem in _list:
            file.write('%s\n' % listitem)


def read_list(filename, data_type = str):
    res = []
    with open(filename, 'r') as file:
        for line in file:
            item = line[:-1]
            res.append(data_type(item))
    return res


for scene in scenes:

    x = np.arange(len(levels_to_test))
    width = 0.35
    fig, ax = plt.subplots()

    cnt = 0
    samples = {}
    for ads in acceleration_structures:
        if not from_file:
            res = []
            for level in levels_to_test:
                #subprocess.run([exe_path, str(scene), str(ads), str(no_samples), str(level)])
                p = subprocess.Popen([exe_path, str(scene), str(ads), str(no_samples), str(level)])

                timer = Timer(timeout, kill, [p])

                try:
                    timer.start()
                    p.communicate()
                finally:
                    timer.cancel()

                if os.path.exists("stats.txt"):
                    f = open("stats.txt")
                    out = f.read().split("\n")
                    res.append(float(out[0]))
                    if level in samples:
                        samples[level] += ", " + out[1]
                    else:
                        samples[level] = out[1]
                    f.close()
                    os.remove("stats.txt")
                else:
                    res.append(0)
                    if level in samples:
                        samples[level] += ", -1"
                    else:
                        samples[level] = "-1"

            write_list(res, "avg_time_s" + str(scene) + "_a" + str(ads) + ".txt")
        else:
            res = read_list("avg_time_s" + str(scene) + "_a" + str(ads) + ".txt", float)

        offset = cnt * width
        rects1 = ax.bar(x - width/2 + offset, res, width, label=ads_names[ads]) # label='Avg sample time - ' + str(no_samples) + " samples")
        cnt += 1

    ax.set_xlabel("Maximum tree depth")
    ax.set_ylabel('Avg time to render 1 sample (Seconds)')

    if not from_file:
        labels = [str(level) + " (" + samples[level] + ")" for level in levels_to_test]
        write_list(labels, "avg_time_s" + str(scene) + ".txt")
    else:
        labels = read_list("avg_time_s" + str(scene) + ".txt")

    ax.set_xticks(x)
    ax.set_xticklabels(labels)
    ax.set_title("Scene: " + scene_names[scene] + ', avg sample time - ' + str(no_samples) + " samples")
    ax.legend()
    plt.xticks(rotation=80)
    fig.tight_layout()
    plt.savefig("avg_time_s" + str(scene) + ".png") #+ "_a" + str(ads) +

