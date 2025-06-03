import os
import re
import csv
from collections import defaultdict

RESULTS_DIR = './'
OUTPUT_CSV = 'tsp_summary.csv'

patterns = {
    'path': re.compile(r'^Path:\s*\n((?:[\d \t]+\n)+)', re.MULTILINE),
    'dist': re.compile(r'^Dist:\s*([0-9.]+)', re.MULTILINE),
    'exec_time': re.compile(r'^Execution time:\s*([0-9.]+)\(ms\)', re.MULTILINE),
    'algo_time': re.compile(r'^TSP algorithm time:\s*([0-9.]+)\(ms\)', re.MULTILINE),
    'elapsed': re.compile(r'^Elapsed \(wall clock\) time \(h:mm:ss or m:ss\):\s*([0-9:.]+)', re.MULTILINE),
    'max_rss': re.compile(r'^Maximum resident set size \(kbytes\):\s*([0-9]+)', re.MULTILINE)
}

def parse_file(filepath):
    with open(filepath, 'r') as f:
        content = f.read()
    
    metrics = {}
    path_match = patterns['path'].search(content)
    if path_match:
        path = path_match.group(1).replace('\n', ' ').strip()  # Merge lines and clean
        metrics['path'] = path

data = defaultdict(list)

def parse_filename(filename):
    base = os.path.splitext(filename)[0]
    parts = base.split('_')
    if len(parts) < 3:
        return None
    try:
        trial = int(parts[-1])
    except ValueError:
        return None
    algorithm = '_'.join(parts[:-2])
    dataset = parts[-2]
    return algorithm, dataset, trial

def extract_fields(content):
    results = {}
    for key, pat in patterns.items():
        match = pat.search(content)
        results[key] = match.group(1).strip() if match else None
    if results['path']:
        results['path'] = ' '.join(results['path'].replace('\n', ' ').split())
    return results

for fname in os.listdir(RESULTS_DIR):
    if not fname.endswith('.txt'):
        continue
    parsed = parse_filename(fname)
    if not parsed:
        continue
    algorithm, dataset, trial = parsed
    with open(os.path.join(RESULTS_DIR, fname), 'r', encoding='utf-8') as f:
        content = f.read()
    fields = extract_fields(content)
    entry = {
        'trial': trial,
        'path': fields['path'],
        'dist': float(fields['dist']) if fields['dist'] else None,
        'exec_time': float(fields['exec_time']) if fields['exec_time'] else None,
        'algo_time': float(fields['algo_time']) if fields['algo_time'] else None,
        'elapsed': fields['elapsed'],
        'max_rss': int(fields['max_rss']) if fields['max_rss'] else None
    }
    data[(algorithm, dataset)].append(entry)

csv_rows = []
for (algorithm, dataset), entries in sorted(data.items()):
    # Organize entries by trial number
    trial_map = {e['trial']: e for e in entries}
    
    # Get paths for trials 1-3
    path_data = {
        f'path_{trial}': trial_map.get(trial, {}).get('path')
        for trial in [1, 2, 3]
    }
    
    # Calculate averages for numerical fields
    num_fields = ['dist', 'exec_time', 'algo_time', 'max_rss']
    avg_data = {}
    for field in num_fields:
        valid_values = [e[field] for e in entries if e[field] is not None]
        if valid_values:
            avg_data[f'avg_{field}'] = sum(valid_values) / len(valid_values)
        else:
            avg_data[f'avg_{field}'] = None
    
    # Get elapsed time from first available trial
    elapsed = next((e['elapsed'] for e in entries if e['elapsed']), None)
    
    csv_rows.append({
        'algorithm': algorithm,
        'dataset': dataset,
        **path_data,
        **avg_data,
        'elapsed': elapsed
    })

# Write to CSV
fieldnames = [
    'algorithm', 'dataset',
    'path_1', 'path_2', 'path_3',
    'avg_dist', 'avg_exec_time', 'avg_algo_time',
    'elapsed', 'avg_max_rss'
]

with open(OUTPUT_CSV, 'w', newline='', encoding='utf-8') as csvfile:
    writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
    writer.writeheader()
    for row in csv_rows:
        # Clean None values to empty strings
        cleaned_row = {
            k: v if v is not None else '' 
            for k, v in row.items()
        }
        writer.writerow(cleaned_row)

print(f"Data aggregation complete. Results saved to {OUTPUT_CSV}")
