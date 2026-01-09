import streamlit as st
import crypto_utils as crypto
import ids
import os
import time


# 🔒 Hide sidebar (pages/tabs)
st.markdown(
    """
    <style>
        [data-testid="stSidebar"] { display: none; }
    </style>
    """,
    unsafe_allow_html=True
)


if "role" not in st.session_state:
    st.session_state.role = None

st.set_page_config(page_title="ProtectX Gateway", layout="centered")


st.set_page_config(page_title="ProtectX Gateway", layout="centered")
st.title("🔐 ProtectX – Secure Agency Gateway")

sender_tab, receiver_tab, admin_tab = st.tabs(
    ["📤 Sender Agency", "📥 Receiver Agency", "🛡 Admin"]
)
ADMIN_PASSWORD = "admin123"   # demo password

# ---------------- SENDER ----------------
with sender_tab:
    st.subheader("Sender Agency (Agency A)")

    file = st.file_uploader("Upload file (any type)", type=None)

    if file and st.button("Encrypt & Send"):
        try:
            file_bytes = file.read()

            public_key = crypto.load_public_key("Keys/agencyB_public.pem")
            aes_key = crypto.generate_aes_key()

            encrypted_data, iv = crypto.aes_encrypt_bytes(file_bytes, aes_key)
            encrypted_key = crypto.rsa_encrypt_aes_key(aes_key, public_key)

            package = iv + encrypted_key + encrypted_data


            st.download_button(
                "Download Encrypted File",
                package,
                file_name=file.name + ".enc"
            )
            st.info("ℹ️ This file can ONLY be decrypted using Agency B's PRIVATE key.")


        except:
            ids.failed_encryption()
            st.error("Encryption failed. IDS logged this event.")

# ---------------- RECEIVER ----------------
with receiver_tab:
    st.subheader("Receiver Agency (Agency B)")

    enc_file = st.file_uploader("Encrypted file", type=["enc"])
    key_file = st.file_uploader("Private key", type=["pem"])

    if enc_file and key_file and st.button("Decrypt"):
        try:
            raw = enc_file.read()
            iv = raw[:16]
            enc_key = raw[16:272]     # 16 + 256
            enc_data = raw[272:]


            private_key = crypto.load_private_key_from_bytes(key_file.read())
            aes_key = crypto.rsa_decrypt_aes_key(enc_key, private_key)

            decrypted_bytes = crypto.aes_decrypt_bytes(enc_data, aes_key, iv)

            st.success("✅ Decryption Successful")
            st.download_button(
                "Download Original File",
                decrypted_bytes,
                file_name="decrypted_file"
            )
            

        except:
            ids.failed_decryption()
            st.error("❌ Wrong private key! IDS logged this attempt.")

# ---------------- ADMIN ----------------
with admin_tab:
    st.subheader("Admin Dashboard")

    st.markdown("### 🔑 Public Keys (Visible to Everyone)")
    st.code(open("Keys/agencyA_public.pem").read())
    st.code(open("Keys/agencyB_public.pem").read())

    st.markdown("---")
    st.markdown("### 🔐 IDS Logs (Admin Only)")

    admin_pass = st.text_input("Enter Admin Password", type="password")

    if admin_pass:
        if admin_pass == ADMIN_PASSWORD:
            st.success("Admin authenticated")

            if os.path.exists("logs/ids.log"):
                st.text(open("logs/ids.log").read())
            else:
                st.text("No alerts yet.")

        else:
            ids.admin_login_failed()
            st.error("Wrong password! IDS logged this attempt.")

