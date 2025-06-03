# 주제 : RSA 키 길이에 따른 보안성과 상관관계 
# 직접 알고리즘 구현 X, Python의 cryptography 라이브러리를 이용하여 구현 
# cryptography 라이브러리 없으면, pip3 install cryptography 설치 

from cryptography.hazmat.primitives.asymmetric import rsa, padding
from cryptography.hazmat.primitives import hashes
from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
from cryptography.hazmat.primitives import padding as sym_padding
from cryptography.hazmat.backends import default_backend
import os, time


# RSA 최대 평문 길이 계산 (OAEP 기준)
def get_max_rsa_plaintext_length(key_size_bits, hash_len=32):
    return (key_size_bits // 8) - 2 * hash_len - 2


# RSA 성능 테스트
def rsa_performance_test(key_size):
    print(f"\n[RSA-{key_size}] 테스트 시작")

    # 사용자 입력 메시지 받기
    user_message = input(f"[RSA-{key_size}] 암호화할 평문 입력: ")
    message_bytes = user_message.encode('utf-8')

    # 최대 허용 길이 확인
    max_len = get_max_rsa_plaintext_length(key_size)
    if len(message_bytes) > max_len:
        print(f" RSA-{key_size}로 암호화 가능한 최대 길이는 {max_len} bytes입니다. 메시지가 잘립니다.")
        message_bytes = message_bytes[:max_len]

    # 1. 키 생성
    start = time.perf_counter()
    private_key = rsa.generate_private_key(public_exponent=65537, key_size=key_size)
    public_key = private_key.public_key()
    end = time.perf_counter()
    print(f"키 생성 시간: {end - start:.4f}초")

    # 2. 암호화
    start = time.perf_counter()
    ciphertext = public_key.encrypt(
        message_bytes,
        padding.OAEP(mgf=padding.MGF1(algorithm=hashes.SHA256()), algorithm=hashes.SHA256(), label=None)
    )
    end = time.perf_counter()
    print(f"암호화 시간: {end - start:.6f}초")

    # 3. 복호화
    start = time.perf_counter()
    decrypted = private_key.decrypt(
        ciphertext,
        padding.OAEP(mgf=padding.MGF1(algorithm=hashes.SHA256()), algorithm=hashes.SHA256(), label=None)
    )
    end = time.perf_counter()
    print(f"복호화 시간: {end - start:.6f}초")
    print(f"복호화 결과: {decrypted.decode('utf-8')}")


# AES + RSA 하이브리드 암호화
def hybrid_encrypt_decrypt():
    print("\n[AES + RSA 하이브리드 암호화 시작]")

    # 사용자 입력
    user_message = input("[하이브리드] 암호화할 평문 입력: ")
    plaintext = user_message.encode('utf-8')

    # 1. AES 키 및 IV 생성
    start = time.perf_counter()
    aes_key = os.urandom(32)  # AES-256
    iv = os.urandom(16)
    end = time.perf_counter()
    print(f"[AES] 키/IV 생성 시간: {end - start:.6f}초")

    # 2. AES 암호화
    start = time.perf_counter()
    padder = sym_padding.PKCS7(128).padder()
    padded_data = padder.update(plaintext) + padder.finalize()

    cipher = Cipher(algorithms.AES(aes_key), modes.CBC(iv), backend=default_backend())
    encryptor = cipher.encryptor()
    aes_ciphertext = encryptor.update(padded_data) + encryptor.finalize()
    end = time.perf_counter()
    print(f"[AES] 메시지 암호화 시간: {end - start:.6f}초")

    # 3. RSA 키 생성 - RSA-2048
    start = time.perf_counter()
    private_key = rsa.generate_private_key(public_exponent=65537, key_size=2048)
    public_key = private_key.public_key()
    end = time.perf_counter()
    print(f"[RSA] 키 생성 시간: {end - start:.6f}초")

    # 4. RSA로 AES 키 암호화
    start = time.perf_counter()
    encrypted_aes_key = public_key.encrypt(
        aes_key,
        padding.OAEP(mgf=padding.MGF1(hashes.SHA256()), algorithm=hashes.SHA256(), label=None)
    )
    end = time.perf_counter()
    print(f"[RSA] AES 키 암호화 시간: {end - start:.6f}초")

    # 5. RSA로 AES 키 복호화
    start = time.perf_counter()
    decrypted_aes_key = private_key.decrypt(
        encrypted_aes_key,
        padding.OAEP(mgf=padding.MGF1(hashes.SHA256()), algorithm=hashes.SHA256(), label=None)
    )
    end = time.perf_counter()
    print(f"[RSA] AES 키 복호화 시간: {end - start:.6f}초")

    # 6. AES 복호화
    start = time.perf_counter()
    decipher = Cipher(algorithms.AES(decrypted_aes_key), modes.CBC(iv), backend=default_backend())
    decryptor = decipher.decryptor()
    padded_plaintext = decryptor.update(aes_ciphertext) + decryptor.finalize()

    unpadder = sym_padding.PKCS7(128).unpadder()
    recovered = unpadder.update(padded_plaintext) + unpadder.finalize()
    end = time.perf_counter()
    print(f"[AES] 메시지 복호화 시간: {end - start:.6f}초")

    print("복호화 결과:", recovered.decode('utf-8'))


# RSA, AES + RSA 방식 실행행
rsa_performance_test(2048) #RSA-2048
rsa_performance_test(4096) #RSA-4096
hybrid_encrypt_decrypt()