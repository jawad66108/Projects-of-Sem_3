import datetime
import os
import socket

LOG_DIR = "logs"
LOG_FILE = os.path.join(LOG_DIR, "ids.log")

os.makedirs(LOG_DIR, exist_ok=True)

def get_ip():
    try:
        return socket.gethostbyname(socket.gethostname())
    except:
        return "UNKNOWN"

def log_event(event_type):
    time = datetime.datetime.now().strftime("Date: %Y-%m-%d   ||   Time: %H:%M:%S")
    ip = get_ip()
    log = f"[{time}] {event_type}   ||  IP: {ip}\n"
    with open(LOG_FILE, "a") as f:
        f.write(log)

def failed_decryption():
    ip = get_ip()
    log_event("FAILED_DECRYPTION", f"IP: {ip}")


def failed_encryption():
    log_event("FAILED_ENCRYPTION")

def admin_login_failed():
    log_event("ADMIN_LOGIN_FAILED")

