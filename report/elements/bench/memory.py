from elements.bench.base import AbstractBench

import matplotlib.pyplot as plt
import numpy as np
import os.path
from matplotlib.ticker import MultipleLocator

class Memory(AbstractBench):
    def __init__(self, obj, bench_obj):
        self.obj = obj
        self.bench_obj = bench_obj

    def to_html(self):
        self.gen_images()
        wd = os.getcwd()

        header = "<h2 id='Memory'>Memory</h2>"

        # Adjusted threshold factor
        threshold_factor = 0.3

        # Theoretical cache sizes in KiB
        theoretical_cache_sizes = [
            self.obj["meta"]["mem_info"]["l1d"] / 1024,
            self.obj["meta"]["mem_info"]["l2"] / 1024,
            self.obj["meta"]["mem_info"]["l3"] / 1024
        ]
        cache_labels = ['L1 Cache', 'L2 Cache', 'L3 Cache']

        # Detect cache sizes and find the closest matches to theoretical sizes
        detected_cache_sizes = self.detect_cache_sizes(self.bench_obj["Results"][0]["Memory_Size"], self.bench_obj["Results"][1]["Latency"], 0.3) / 1024
        detected_cache_sizes.sort()
        closest_cache_sizes = self.find_closest_cache_sizes(detected_cache_sizes, theoretical_cache_sizes)

        # Create the HTML table
        table = "<table><tr><th>Cache</th><th>Theoretical Size (KiB)</th><th>Detected Size (KiB)</th></tr>"
        for label, theoretical_size, detected_size in zip(cache_labels, theoretical_cache_sizes, closest_cache_sizes):
            table += f"<tr><td>{label}</td><td>{theoretical_size}</td><td>{detected_size}</td></tr>"
        table += "</table>"

        imgs = f"<img src='{wd}/out/cache_latency.png'/>"
        return header + imgs + table

    def gen_images(self):

        # Load data from JSON file
        data = self.obj

        # Extract memory sizes and latencies from the data
        memory_sizes = self.bench_obj["Results"][0]["Memory_Size"]
        latencies = self.bench_obj["Results"][1]["Latency"]

        # Convert lists to numpy arrays for easier manipulation
        memory_sizes = np.array(memory_sizes)
        latencies = np.array(latencies)

        # Convert memory sizes to KiB
        memory_sizes_kib = memory_sizes / 1024

        # Theoretical cache sizes in KiB
        theoretical_cache_sizes = [
            data["meta"]["mem_info"]["l1d"] / 1024,
            data["meta"]["mem_info"]["l2"] / 1024,
            data["meta"]["mem_info"]["l3"] / 1024
        ]

        # Generate warnings for aberrations and print results to console
        threshold_factor = 0.3  # Adjusted threshold factor
        for i in range(len(latencies) - 1):
            diff = latencies[i + 1] - latencies[i]
            if abs(diff) > threshold_factor * np.mean(np.abs(np.diff(latencies))):
                warning_message = f"Aberration detected between indices {i} and {i + 1}: difference = {diff}"
                #print(warning_message)

        # Detect cache sizes based on significant changes in latency
        cache_sizes = self.detect_cache_sizes(memory_sizes, latencies, threshold_factor)  / 1024
        cache_sizes.sort()
        closest_cache_sizes = self.find_closest_cache_sizes(cache_sizes, theoretical_cache_sizes)

        # # Display detected cache sizes
        # print("\nDetected cache sizes (in KiB):", cache_sizes / 1024)
        # print("\nClosest detected cache sizes to theoretical sizes (in KiB):", closest_cache_sizes)

        # Plot the data
        plt.figure(figsize=(10, 6))
        plt.plot(memory_sizes_kib, latencies, marker='o', linestyle='-', label='Data')
        cache_labels = ['L1 Cache', 'L2 Cache', 'L3 Cache']
        for theoretical_size, label in zip(theoretical_cache_sizes, cache_labels):
            plt.axvline(x=theoretical_size, color='green', linestyle='--', label=f'Theoretical {label} size: {theoretical_size} KiB')

        # Set logarithmic scales for both axes
        plt.xscale("log", base=2)
        plt.yscale("log", base=10)
        plt.xlabel("Buffer Size (KiB)")
        plt.ylabel("Latency (ns)")
        plt.title("Cache Latency")
        plt.grid(True, which="both", ls="--")
        plt.legend()

        # Save the plot as an image file
        plt.savefig("out/cache_latency.png")

    def get_index(self):
        return "<li><a href='#Memory'>Memory</a></li>"
    
    threshold_factor = 0.3  # Adjusted threshold factor
# Function to detect significant changes in latency
    def detect_cache_sizes(self, memory_sizes, latencies, threshold_factor=2):
        changes = np.diff(latencies)
        threshold = threshold_factor * np.mean(np.abs(changes))
        significant_changes = np.where(np.abs(changes) > threshold)[0]
        cache_sizes = np.array([])
        for i in significant_changes:
            # cache_sizes += (memory_sizes[i])
            cache_sizes = np.append(cache_sizes, memory_sizes[i])
        return cache_sizes
    
# Function to find the closest detected cache sizes to the theoretical sizes
    def find_closest_cache_sizes(self, detected_cache_sizes, theoretical_cache_sizes):
        closest_cache_sizes = []
        for theoretical_size in theoretical_cache_sizes:
            closest_size = min(detected_cache_sizes, key=lambda x: abs(x - theoretical_size))
            closest_cache_sizes.append(closest_size)
        return closest_cache_sizes
