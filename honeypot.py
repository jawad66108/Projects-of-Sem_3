import streamlit as st
import time
import datetime
import socket
import os

# ---------------- CONFIG ----------------
st.set_page_config(page_title="ProtectX Gateway", layout="centered")

# Hide sidebar
st.markdown(
    """
    <style>
        [data-testid="stSidebar"] { display: none; }
    </style>
    """,
    unsafe_allow_html=True
)

LOG_FILE = "logs/honeypot.log"
os.makedirs("logs", exist_ok=True)

# ---------------- HELPERS ----------------
def get_ip():
    try:
        return socket.gethostbyname(socket.gethostname())
    except:
        return "UNKNOWN"

def log(msg):
    with open(LOG_FILE, "a") as f:
        f.write(f"[{datetime.datetime.now()}] {msg}\n")

# ---------------- SESSION INIT ----------------
if "start_time" not in st.session_state:
    st.session_state.start_time = time.time()

if "expired" not in st.session_state:
    st.session_state.expired = False

# Session start time (date + time)
if "entry_time" not in st.session_state:
    st.session_state.entry_time = datetime.datetime.now().strftime(
        "%Y-%m-%d %H:%M:%S"
    )


if "actions" not in st.session_state:
    st.session_state.actions = {
        "Upload File": 0,
        "Encrypt & Send": 0,
        "Decrypt File": 0,
        "View Public Keys": 0
    }

ip = get_ip()
elapsed = time.time() - st.session_state.start_time

# ---------------- TIMEOUT ----------------
if elapsed >= 15:
    st.session_state.expired = True

# ---------------- SESSION EXPIRED ----------------
if st.session_state.expired:
    print("====== HONEYPOT SESSION ENDED ======")

    print(f"Time/Date : {st.session_state.entry_time}")
    print(f"IP Address : {ip}")
    print("Actions Performed:")
    for action, count in st.session_state.actions.items():
        print(f"  - {action}: {count} times")
    print("===================================")

    log(f"Honeypot ended | IP: {ip} | Actions: {st.session_state.actions}")

    st.error("❌ ACCESS DENIED")
    st.markdown("### ERROR 404 – Unauthorized Access Detected")
    st.markdown("Your session has been terminated.")
    st.stop()

# ---------------- REALISTIC UI ----------------
st.title("🔐 ProtectX – Secure Agency Gateway")

sender_tab, receiver_tab, admin_tab = st.tabs(
    ["📤 Sender Agency", "📥 Receiver Agency", "🛡 Admin"]
)

# ---------- SENDER TAB ----------
with sender_tab:
    st.subheader("Sender Agency (Agency A)")

    if st.button("Upload File"):
        st.session_state.actions["Upload File"] += 1
        st.write("Uploading file...")

    if st.button("Encrypt & Send"):
        st.session_state.actions["Encrypt & Send"] += 1
        st.write("Encrypting and sending file...")

# ---------- RECEIVER TAB ----------
with receiver_tab:
    st.subheader("Receiver Agency (Agency B)")

    if st.button("Decrypt File"):
        st.session_state.actions["Decrypt File"] += 1
        st.write("Decrypting file...")


# ---------- ADMIN TAB ----------
with admin_tab:
    st.subheader("Admin Dashboard")

    if st.button("View Public Keys"):
        st.session_state.actions["View Public Keys"] += 1

        st.markdown("### 🔑 Public Keys")
        st.code(open("Keys/agencyA_public.pem").read())
        st.code(open("Keys/agencyB_public.pem").read())

st.warning("Session active...")
