"""
<Edit Distance Measurement>

Edit Distance Measurement
2 개의 DNA 시퀀스 간의 Edit Distance 계산하는 문제 

<주의>
대소문자를 구분 X
입력 파일 안에 아무것도 없는 경우 "No DNA sequence" 출력
입력 파일에 2 개 이상의 서열이 포함되어 있는 경우, 
거리를 측정하기 위해 첫 번째의 2 개의 서열을 사용
입력 파일에 2 개 미만의 서열이 포함되어 있는 경우,
"Need more sequences" 출력 
시퀀스 중 하나라도 DNA 서열이 아닌 경우 "No DNA sequence"를 출력합니다.
DNA 서열은 "A", "T", "C", "G"만 포함
공백을 포함한 특수 문자를 허용 x
FASTA 형식을 따르지 않는 경우 "No correct format"을 출력.
"""
#필요한 라이브러리 
import sys,time
#시간 계산
time_Start = time.time()

#fasta형식의 함수  ">"는 주석처리한다 
def Fasta_R(input_Fname):
    sequences = []
    with open(input_Fname, "r") as file:
        seq = ""
        for line in file:
            if line.startswith(">"): # >는 시퀀스 시작하는 줄 
                if seq: # 이전 시퀀스가 있으면 추가 
                    sequences.append(seq.upper())
                seq = "" 
            else: #시퀀스 내용 저장 및 대문자로 변환 
                seq += line.strip().upper()
        if seq: #마지막 시퀀스 추가
            sequences.append(seq)
    #시퀀스 서열이 부족한 경우  프로그램 종료
    if len(sequences) < 2:
        print("Need more sequences")
        sys.exit(1)
    #시퀀스 atcg외의 글자가 있는 경우 오류처리 종료 
    for seq in sequences[:2]:
        if not seq or any(char.upper() not in set("ATCG") for char in seq):
            print("No DNA sequence")
            sys.exit(1)    
        if ">" in seq: #시퀀스가 파스타 형식이 아닌 경우의 오류처리하고 종료
            print("No correct format")
            sys.exit(1)

    return sequences
 
# DNA 시퀀스인지 확인하는 함수
# 시퀀스의문자가 모두 DNA 문자인지 확인 
def is_dna(Seq):
    dna_char = set("ATCG")
    return all(char.upper() in dna_char for char in Seq)

#메인 부분 
#커맨드아규먼트라인 이용해서 입출력파일 받기
if len(sys.argv) != 2:
    print("입력해주세요 : python Assignment4.py 입력파일")
    sys.exit(1)

input_file = sys.argv[1]

try:
    Sequences = Fasta_R(input_file)
except FileNotFoundError:
    print("입력파일을 찾을 수 없습니다.")
    sys.exit(1)


# 2개의 시퀀스를 가져온다. 
seq1, seq2 = Sequences[:2]

#시퀀스가 대소문자 구분 없이 DNA서열인지 아닌지를 확인한다. 
if not all(is_dna(seq) for seq in [seq1, seq2]):
    print("No DNA sequence")
    sys.exit(1)


# DP 테이블 초기화 
len1, len2 = len(seq1), len(seq2)
dp = [[0] * (len2 + 1) for _ in range(len1 + 1)]

for i in range(len1 + 1):
    dp[i][0] = i
for j in range(len2 + 1):
    dp[0][j] = j

# DP 테이블을 사용하여 Edit Distance를 계산
for i in range(1, len1 + 1):
    for j in range(1, len2 + 1):
        if seq1[i - 1] == seq2[j - 1]:
            dp[i][j] = dp[i - 1][j - 1]
        else:
            dp[i][j] = min(dp[i - 1][j - 1], dp[i - 1][j], dp[i][j - 1]) + 1

distance = dp[len1][len2]


time_End = time.time()

End_Time_ms = int((time_End - time_Start) * 10**6)

#출력결과를 cmd 화면에 보여준다. 
print("Edit Distance : {} ".format(distance))
print("걸린 시간 (ms): ", End_Time_ms)