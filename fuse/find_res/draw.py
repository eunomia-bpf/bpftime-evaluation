def extract_real_time(line):
    """Extract the real time value from a line of text."""
    if 'real' in line:
        parts = line.split()
        real_time_str = parts[1]  # real time is the second part
        minutes, seconds = real_time_str.split('m')
        real_time = float(minutes) * 60 + float(seconds.rstrip('s'))  # convert to seconds
        return real_time
    return None

def calculate_average_real_time(filename):
    """Calculate and print the average real time from a file."""
    total_real_time = 0
    count = 0

    with open(filename, 'r') as file:
        for line in file:
            real_time = extract_real_time(line)
            if real_time is not None:
                total_real_time += real_time
                count += 1

    if count > 0:
        avg_real_time = total_real_time / count
        print(f"Average Real Time: {avg_real_time:.3f} seconds")
    else:
        print("No valid real time data found in the file.")

# Usage example
# filename = 'path_to_your_file.txt'
# calculate_average_real_time(filename)

# Usage
print("Average real time for find no cache:")
filename = 'find_fuse.txt'
calculate_average_real_time(filename)
print("Average real time for find with cache:")
filename = 'find_cache.txt'
calculate_average_real_time(filename)