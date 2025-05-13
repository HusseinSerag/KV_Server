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

    cmdReq = command[0]

    if cmdReq in {"keys", "table", "size", "capacity"}:
        msgType = 0x00
        totalLen = 2 + 4 + len(cmdReq)
        packed = struct.pack("<IHI", totalLen, msgType, len(cmdReq))
        return packed + cmdReq.encode()

    elif cmdReq.startswith("i_"):
        # Integer request
        command[0] = command[0][2:]  # Remove 'i_'
        msgType = 0x02
        totalLen = 2
        val = 0
        isNum = False

        if len(command) > 1 and is_number(command[-1]):
            val = int(command[-1])
            command = command[:-1]
            totalLen += 8  # int64

        for s in command:
            totalLen += 4 + len(s)

        if totalLen > k_max_msg:
            return None

        packed = struct.pack("<IH", totalLen, msgType)
        for s in command:
            packed += struct.pack("<I", len(s)) + s.encode()

        if isNum or val != 0:
            packed += struct.pack("<q", val)  # int64

        return packed

    elif cmdReq.startswith("d_"):
        # Double request
        command[0] = command[0][2:]
        msgType = 0x01
        totalLen = 2
        val = 0.0
        isNum = False

        if len(command) > 1 and is_number(command[-1]):
            val = float(command[-1])
            command = command[:-1]
            totalLen += 8  # double

        for s in command:
            totalLen += 4 + len(s)

        if totalLen > k_max_msg:
            return None

        packed = struct.pack("<IH", totalLen, msgType)
        for s in command:
            packed += struct.pack("<I", len(s)) + s.encode()

        if isNum or val != 0.0:
            packed += struct.pack("<d", val)

        return packed

    elif cmdReq.startswith("s_"):
        # String request
        command[0] = command[0][2:]
        msgType = 0x03
        totalLen = 2

        for s in command:
            totalLen += 4 + len(s)

        if totalLen > k_max_msg:
            return None

        packed = struct.pack("<IH", totalLen, msgType)
        for s in command:
            packed += struct.pack("<I", len(s)) + s.encode()

        return packed

    else:
        return None

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

