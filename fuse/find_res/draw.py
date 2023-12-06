def extract_time_values(line):
    """Extract time values from a line of text."""
    parts = line.split()
    real_time = float(parts[1].replace('m', '').replace('s', ''))
    user_time = float(parts[3].replace('m', '').replace('s', ''))
    sys_time = float(parts[5].replace('m', '').replace('s', ''))
    return real_time, user_time, sys_time

def calculate_averages(filename):
    """Calculate and print average times from a file."""
    total_real, total_user, total_sys = 0, 0, 0
    count = 0

    with open(filename, 'r') as file:
        for line in file:
            if line.strip().startswith('real'):
                real, user, sys = extract_time_values(line)
                total_real += real
                total_user += user
                total_sys += sys
                count += 1

    if count > 0:
        avg_real = total_real / count
        avg_user = total_user / count
        avg_sys = total_sys / count
        print(f"Average Real Time: {avg_real} seconds")
        print(f"Average User Time: {avg_user} seconds")
        print(f"Average Sys Time: {avg_sys} seconds")
    else:
        print("No data found in the file.")

# Usage
filename = 'find_fuse.txt'
calculate_averages(filename)