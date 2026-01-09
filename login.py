import streamlit as st

st.set_page_config(page_title="ProtectX Login")

# 🔒 Hide sidebar tabs
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

st.title("🔐 ProtectX Gateway")

username = st.text_input("Username")
password = st.text_input("Password", type="password")

if st.button("Login"):
    if username == "" or password == "":
        st.error("Please enter username and password")

    elif username == "admin" and password == "secure123":
        st.session_state.role = "real"
        st.switch_page("pages/app.py")

    else:
        st.session_state.role = "honeypot"
        st.switch_page("pages/honeypot.py")
