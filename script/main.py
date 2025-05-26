import random
import string
import socket
import time
import threading
from command import parse_command, parse_resp, send_all
import random
import string
import multiprocessing
import os
import signal
import datetime
import logger

# --- Random Generators ---

def random_string(length=5):
    return ''.join(random.choices(string.ascii_letters, k=length))

def random_int():
    return str(random.randint(-10000, 10000))

def random_double():
    while True:
        val = round(random.uniform(-10000.0, 10000.0), 2)
        if not (val != val or val == float('inf') or val == float('-inf')):  # Avoid NaN and inf
            return str(val)

# --- Command Generator ---

def generate_random_command():
    key = random_string()

    command_type = random.choice([
        "set", "get",  "inc", "dec", "mult", "len", "concat", "keys", "unknown", "div", "del"
    ])

    if command_type == "set":
        value_type = random.choice(["int", "double", "string"])
        if value_type == "int":
            value = random_int()
        elif value_type == "double":
            value = random_double()
        else:
            value = random_string()
        return ["set", key, value]


    # elif command_type == "ladd":
    #     return   [command_type, key, random_string()]    
    # elif command_type == "remove":
    #     return [command_type, key, "index", random_int()]
    # elif command_type == "update":
    #     return [command_type, key, "index", random_int(), random_string()]
    elif command_type in ["get", "inc", "dec", "len"]:
        return [command_type, key]

    elif command_type == "mult":
        return ["mult", key, random_int()]

    elif command_type == "concat":
        return ["concat", key, random_string()]

    elif command_type == "keys":
        return ["keys"]

    elif command_type == "unknown":
        value = random.choice([random_string(), random_int(), random_double()])
        return [random_string(), key, value]

    else:
        return None
HOST = "127.0.0.1"
PORT = random.randint(3000,4000)
def run_server(log_path):
    os.system(f"cd && cd KV_Server && ./start_script.sh {PORT} 0 > {log_path} 2>&1")


def handle_command(command, results,cb,timings):
    msg = parse_command(command)
    if not msg:
        return
    start = time.time()
    try:
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            s.connect((HOST, PORT))
            send_all(s, msg)
            parse_resp(s, cb)
            elapsed = time.time() - start
            timings.append(elapsed)  # record latency
            results["success"] += 1
    except Exception:
        results["fail"] += 1


def run_test(n=1000):
    print("testing underway...")
    base_dir = "tests"
    os.makedirs(base_dir, exist_ok=True)

    dir_name = os.path.join(base_dir, f"test_{datetime.datetime.now(tz=datetime.timezone.utc).strftime('%Y-%m-%dT%H:%M:%S%z')}")
    os.makedirs(dir_name)
    process = multiprocessing.Process(target=run_server, args=(os.path.join(dir_name, "server.output.log"),))
    process.start()
    time.sleep(5)
    threads = []
    results = {"success": 0, "fail": 0}
    client_logger = logger.setup_logger("client_logger", os.path.join(dir_name, "client.output.log"))
    
    timings = []  # store per-command latencies
    
    start_time = time.time()
    for _ in range(n):
        command = generate_random_command()
        t = threading.Thread(
            target=handle_command,
            name=f"Thread {_}",
            args=(command, results, lambda msg: client_logger.info("%s", msg.replace("%", " ")), timings)
        )
        threads.append(t)
        t.start()

    for t in threads:
        t.join()

    duration = time.time() - start_time
    
    # Calculate latency stats
    if timings:
        avg_latency = sum(timings) / len(timings)
        max_latency = max(timings)
        min_latency = min(timings)
    else:
        avg_latency = max_latency = min_latency = 0
    
    with open(os.path.join(dir_name, "results.txt"), "w") as f:
        f.write("Load Test Complete\n")
        f.write(f"Success: {results['success']}\n")
        f.write(f"Failures: {results['fail']}\n")
        f.write(f"Total Time: {duration:.2f}s\n")
        f.write(f"Average Time per Command (wall time): {duration / n:.5f}s\n")
        f.write(f"Average Latency per Command (per-thread): {avg_latency:.5f}s\n")
        f.write(f"Max Latency: {max_latency:.5f}s\n")
        f.write(f"Min Latency: {min_latency:.5f}s\n")

    os.kill(process.pid, signal.SIGTERM)
    process.join()
    print("Test done, check results")

if __name__ == "__main__":
    run_test(5000)
