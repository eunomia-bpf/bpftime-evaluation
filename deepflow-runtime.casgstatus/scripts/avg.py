# Function to calculate the averages for 'request' and 'transfer' for each size
def calculate_averages(data):
    averages = {}
    for sub_key in data:
        size_sum = {}
        size_count = {}

        # Process each list of entries
        for entry_list in data[sub_key]['details']:
            for item in entry_list:
                size = item['size']
                request = item['request']
                transfer = item['transfer']

                if size not in size_sum:
                    size_sum[size] = {'request': 0, 'transfer': 0}
                    size_count[size] = 0

                size_sum[size]['request'] += request
                size_sum[size]['transfer'] += transfer
                size_count[size] += 1

        # Calculate averages
        averages[sub_key] = {}
        for size in size_sum:
            averages[sub_key][size] = {
                'average_request': size_sum[size]['request'] / size_count[size],
                'average_transfer': size_sum[size]['transfer'] / size_count[size]
            }

    return averages

# Calculating averages for 'https' and 'http'
https_averages = calculate_averages(https_structure)
http_averages = calculate_averages(http_structure)

https_averages, http_averages

