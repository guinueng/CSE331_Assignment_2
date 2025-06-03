import subprocess
from pathlib import Path

executables = [
    "./2_Approximation_Algorithm",
    "./k-means",
    "./k-means_mod",
    "./min_weight_loop_w_insertion",
    "./Held-Karp_algorithm",
    "./brute_force"
]

datasets = [
    "a5.tsp",
    "a25.tsp",
    "a100.tsp",
    "a280.tsp",
    "xql662.tsp",
    "kz9976.tsp",
    "mona-lisa100K.tsp"
]

num_trials = 3  # Set the number of trials you want

for exe in executables:
    exe_name = Path(exe).name
    for dataset in datasets:
        for trial in range(1, num_trials + 1):
            output_file = f"{exe_name}_{dataset}_{trial}.txt"
            # Build the command string
            cmd = f"/usr/bin/time -v {exe} ../dataset/{dataset}"
            print(f"Running: {cmd} -> {output_file}")
            # Run the command, redirecting both stdout and stderr to the output file
            with open(output_file, "w") as f:
                result = subprocess.run(cmd, shell=True, stdout=f, stderr=subprocess.STDOUT)
                if result.returncode != 0:
                    print(f"Error running {exe_name} on {dataset} (trial {trial})")
