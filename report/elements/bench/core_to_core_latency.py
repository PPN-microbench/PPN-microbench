from elements.bench.base import AbstractBench

import matplotlib.pyplot as plt
import matplotlib.colors as mcolors
import numpy as np
import os.path

graphs = 1 # 1 For only min, 2 for min and mean

class CoreToCoreLatency(AbstractBench):
    def __init__(self, obj, bench_obj):
        self.obj = obj
        self.bench_obj = bench_obj

    def to_html(self):
        self.gen_images()
        wd = os.getcwd()

        header = "<h2 id='CoreToCoreLatency'>Core-To-Core Latency</h2>"

        minImgs = f"<img src='{wd}/out/core_to_core_latency_min.svg'/>"
        # minTitle = "<center><p>Core-to-core minimum latency graph in nanoseconds</p></center>"

        res = header + minImgs # + minTitle
        
        if graphs == 2 :
            meanImgs = f"<img src='{wd}/out/core_to_core_latency_mean.svg'/>"
            # meanTitle = "<center><p>Core-to-core mean latency graph in nanoseconds</p></center>"
            res += meanImgs # + meanTitle

        return res

    def gen_images(self):
        for m in range(graphs) :
            if m == 0 :
                mode = "min"
            elif m == 1 :
                mode = "mean"
            data = self.bench_obj["results"][mode]

            matrix = np.array(data)

            fig, ax = plt.subplots(figsize=(10, 10))
            plt.tight_layout(pad=3.5)

            colors = plt.cm.plasma(np.linspace(0, 1, 256))
            colors[0] = [0.3, 0.3, 0.3, 1]
            new_cmap = mcolors.ListedColormap(colors)
            cax = ax.imshow(matrix, cmap=new_cmap, interpolation="nearest", aspect='auto')

            plt.figtext(0.5, 0.02, "Core-to-core latency", horizontalalignment = 'center', size='x-large')

            # Show values
            if (matrix.shape[0] <= 32) :
                for i in range(matrix.shape[0]):
                    for j in range(matrix.shape[1]):
                        ax.text(j, i, str(matrix[i, j] if matrix[i, j] != 0 else ""), ha='center', va='center', color='black')

            # Axis options
            fig_width, _ = fig.get_size_inches() * fig.dpi

            # 1 tick every 10 pixels
            tick_spacing = int(matrix.shape[0] / (fig_width / 10) + 1)
            tick_indices = np.arange(0, matrix.shape[0], tick_spacing)
            tick_indices = np.arange(0, matrix.shape[1], tick_spacing)

            # ax.set_yticklabels(f"Core {j}" for j in range(matrix.shape[0]))
            ax.xaxis.set_ticks_position('top')
            ax.tick_params(axis='x', labeltop=True, labelbottom=False)
            ax.set_aspect('equal')

            fig.colorbar(cax, fraction=0.03, pad=0.04, label="Time (ns)")

            plt.tight_layout(pad=4)

            plt.savefig("out/core_to_core_latency_" + mode + ".svg")
            plt.clf()
            plt.close()

    def get_index(self):
        return "<li><a href='#CoreToCoreLatency'>Core-to-core latency</a></li>"
