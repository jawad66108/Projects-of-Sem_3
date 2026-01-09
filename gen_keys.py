# generate_keys.py
# This file creates RSA keys for agencies (run only once)

from cryptography.hazmat.primitives.asymmetric import rsa
from cryptography.hazmat.primitives import serialization
import os


def generate_agency_keys(agency_name):
    """
    Generates RSA public and private keys for an agency
    """

    # 1. Create RSA private key
    private_key = rsa.generate_private_key(
        public_exponent=65537,
        key_size=2048
    )

    # 2. Get public key from private key
    public_key = private_key.public_key()

    # 3. Save PRIVATE key (DO NOT SHARE)
    with open(f"{agency_name}_private.pem", "wb") as f:
        f.write(
            private_key.private_bytes(
                encoding=serialization.Encoding.PEM,
                format=serialization.PrivateFormat.PKCS8,
                encryption_algorithm=serialization.NoEncryption()
            )
        )

    # 4. Save PUBLIC key (CAN BE SHARED)
    with open(f"{agency_name}_public.pem", "wb") as f:
        f.write(
            public_key.public_bytes(
                encoding=serialization.Encoding.PEM,
                format=serialization.PublicFormat.SubjectPublicKeyInfo
            )
        )

    print(f"Keys generated for {agency_name}")


# -------- MAIN EXECUTION --------
if __name__ == "__main__":

    # Create keys folder if not exists
    if not os.path.exists("."):
        os.mkdir(".")

    # Generate keys for two agencies
    generate_agency_keys("agencyA")
    generate_agency_keys("agencyB")

    print("\nKey generation complete!")
