from elements.bench.base import AbstractBench

import matplotlib.pyplot as plt
import numpy as np
import os.path

class Ops(AbstractBench):
    def __init__(self, obj, bench_obj):
        self.obj = obj
        self.bench_obj = bench_obj

        self.data = np.array(self.bench_obj["results"])

        ops_count = self.bench_obj["ops_count"]
        cpus = self.obj["meta"]["cpu_info"]["cpus"]

        self.data = 1e9 / self.data * ops_count * cpus

    def to_html(self):
        self.gen_images()
        wd = os.getcwd()

        # data[0] -> int 32
        # data[1] -> int 64
        # data[2] -> float 32
        # data[3] -> float 64
        # data[4] -> int 64 simd
        # data[5] -> float 64 simd

        t = []

        for e in self.data / 1e9:
            t.append([
                np.average(e),
                np.amax(e),
                np.std(e)
            ])

        header = "<h2 id='Ops'>Integer and Floating point operations per seconds</h2>"
        p1 = "<p> All values are in Giga operation/instruction per seconds. SIMD operation are generated using openmp's #pragma omp simd directive. </p>"

        table = "<tr><th></th><th>Average (GIPS)</th><th>Max (GIPS)</th><th>Std. (GIPS)</th></tr>"
        table += "<tr><th>i32</th><td>{:10.4f}</td><td>{:10.4f}</td><td>{:10.4f}</td></tr>".format(t[0][0], t[0][1], t[0][2])
        table += "<tr><th>i64</th><td>{:10.4f}</td><td>{:10.4f}</td><td>{:10.4f}</td></tr>".format(t[1][0], t[1][1], t[1][2])
        table += "<tr><th>f32</th><td>{:10.4f}</td><td>{:10.4f}</td><td>{:10.4f}</td></tr>".format(t[2][0], t[2][1], t[2][2])
        table += "<tr><th>f64</th><td>{:10.4f}</td><td>{:10.4f}</td><td>{:10.4f}</td></tr>".format(t[3][0], t[3][1], t[3][2])
        table += "<tr><th>i64 SIMD</th><td>{:10.4f}</td><td>{:10.4f}</td><td>{:10.4f}</td></tr>".format(t[4][0], t[4][1], t[4][2])
        table += "<tr><th>f64 SIMD</th><td>{:10.4f}</td><td>{:10.4f}</td><td>{:10.4f}</td></tr>".format(t[5][0], t[5][1], t[5][2])
        table = f"<table>{table}</table>"

        imgs = f"<img src='{wd}/out/ops_scalar.png'/></br><img src='{wd}/out/ops_simd.png'/>"
        
        return header + p1 + table

    def gen_images(self):
        x = [i for i in range(len(self.data[0]))]

        # SCALAR BENCH

        # plt.yscale("log")

        plt.plot(x, self.data[0] / 1e9)
        plt.plot(x, self.data[1] / 1e9)
        plt.plot(x, self.data[2] / 1e9)
        plt.plot(x, self.data[3] / 1e9)

        plt.savefig("out/ops_scalar.png")
        plt.clf()

        # SIMD BENCH

        # plt.yscale("log")

        plt.plot(x, self.data[4] / 1e9)
        plt.plot(x, self.data[5] / 1e9)

        plt.savefig("out/ops_simd.png")
        plt.clf()




    def get_index(self):
        return "<li><a href='#Ops'>Integer and Floating point operations per seconds</a></li>"