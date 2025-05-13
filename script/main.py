import random
import string
import socket
import time
import threading
from command import parse_command, parse_resp, send_all

HOST = "127.0.0.1"
PORT = 3000

def random_key():
    return ''.join(random.choices(string.ascii_lowercase, k=random.randint(3, 8)))

def random_int():
    return str(random.randint(-10000, 10000))

def random_double():
    return str(round(random.uniform(-10000.0, 10000.0), 4))

def random_string():
    return ''.join(random.choices(string.ascii_letters, k=random.randint(5, 12)))

def generate_valid_command():
    key = random_key()

    command_type = random.choice(["i", "d", "s", "generic", "mult"])

    if command_type == "i":
        return ["i_set", key, random_int()]
    elif command_type == "d":
        return ["d_set", key, random_double()]
    elif command_type == "s":
        return ["s_set", key, random_string()]
    elif command_type == "generic":
        return [random.choice(["keys", "table", "capacity", "size"])]
    elif command_type == "mult":
        return [random.choice(["i_mult", "d_mult"]), key, random_int()]
    else:
        return None

def handle_command(command, results):
    msg = parse_command(command)
    if not msg:
        return

    try:
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            s.connect((HOST, PORT))
            send_all(s, msg)
            
            parse_resp(s)
            results["success"] += 1
    except Exception:
        results["fail"] += 1

def run_test(n=1000):
    threads = []
    results = {"success": 0, "fail": 0}
    start_time = time.time()

    for _ in range(n):
        command = generate_valid_command()
        t = threading.Thread(target=handle_command, args=(command, results))
        threads.append(t)
        t.start()

    for t in threads:
        t.join()

    duration = time.time() - start_time
    print("\nLoad Test Complete")
    print(f"Success: {results['success']}")
    print(f"Failures: {results['fail']}")
    print(f"Total Time: {duration:.2f}s")
    print(f"Average Time per Command: {duration / n:.5f}s")

if __name__ == "__main__":
    run_test(5000)
