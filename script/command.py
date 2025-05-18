import struct 

k_max_msg = 4096
def is_number(x):
    try:
        if int(x) or float(x):
            return True
    except ValueError:
        return False
def parse_command(command):
    
    if not command:
        return

    totalLen = 4
    for req in command:
        totalLen += 4 + len(req)
    if totalLen > 4096:
        return 
    packed = struct.pack("<II",totalLen,len(command))
    for s in command:
        packed += struct.pack("<I", len(s)) + s.encode()
    return packed

def recv_all(socket, length):
    """read exactly length data"""
    data = b""
    while len(data) < length :
        more = socket.recv(length - len(data))
        if not more:
            raise EOFError("socket closed")
        data += more
    return data
def parse_resp(socket):
   
    header = recv_all(socket, 4)
    (resp_len,) = struct.unpack("<I",header);

    payload = recv_all(socket, resp_len)
    status = struct.unpack("<I",payload[:4])[0]

    data = payload[4:]

    print(f"Status: {status}- {data.decode()}")


def send_all(sock, data: bytes):
    total_sent = 0
    while total_sent < len(data):
        sent = sock.send(data[total_sent:])
        if sent == 0:
            raise RuntimeError("error sending")
        total_sent += sent

