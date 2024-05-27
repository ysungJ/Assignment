""""
바이오 컴퓨팅 - 과제 8 Approximate Pattern Finding

Note
The input sequence is case-insensitive.
If the input file in a command-line argument does not exist, then print "No input file".
If the input file has nothing, then print "No DNA sequence".
If the input file contains a single sequence, then print "Need more sequences".
If any sequence in the input file is not a DNA sequence, then print "No DNA sequence".
If any sequence in the input file does not follow the fasta format, then print "No correct format".
If multiple approximate patterns of length m having the same similarity are found, then print all the patterns (each pattern per line).
Coding competition: The fastest python code which runs correctly will have a 10% extra credit.

"""
import sys, time

# Fasta 형식을 읽는 함수
def Fasta_R(Input_file):
    Sequences = []
    Correct_format = False # 포맷이 올바른지 확인하기 위한 변수 
    with open(Input_file, "r") as f:
        Sequence = ""
        for line in f:
            if line.startswith(">"):
                Correct_format = True   #  ">"로 시작하는 줄이 나타나면 형식이 올바른 포맷
                if Sequence:
                    Sequences.append(Sequence.upper())  # 대소문자 구별
                    Sequence = ""
            else:
                Sequence += line.strip()  # 줄바꿈 문자를 제거하고 추가
        if Sequence:
            Sequences.append(Sequence.upper())  # 마지막 서열을 리스트에 추가

    if not Correct_format:
        print("No correct format")  # 형식이 올바르지 않은 경우
        sys.exit(1)
    
    return Sequences

# m의 모든 서브스트링을 생성하는 함수
def Get_substrings(sequence, m):
    substrings = []
    for i in range(len(sequence) - m + 1):
        substrings.append(sequence[i:i+m])
    return substrings

# 해밍 거리 계산 함수
# 두 문자열의 길이는 같다고 가정.
# 두 문자열 사이의 해밍 거리를 계산하여 반환
def Hamming_Distance(str1, str2):
    distance = 0
    for i in range(len(str1)):
        if str1[i] != str2[i]:
            distance += 1
    return distance

# 입력한 DNA 서열에서 가장 유사한 패턴을 찾는 함수
def Approximate_patterns_Finding(sequences, m):
    min_distance = float("inf")
    best_pattern = None

    # 첫 번째 서열에서 모든 가능한 서브스트링을 생성합니다.
    for substring in Get_substrings(sequences[0], m):
        total_distance = 0

        # 나머지 서열에서 각각의 서브스트링과의 해밍 거리를 계산하여 누적합니다.
        for sequence in sequences[1:]:
            min_distance_substring = min(Hamming_Distance(substring, subsequence) for subsequence in Get_substrings(sequence, m))
            total_distance += min_distance_substring

        # 누적 거리가 현재까지 최소 거리보다 작으면 최소 거리와 최적 패턴을 갱신합니다.
        if total_distance < min_distance:
            min_distance = total_distance
            best_pattern = substring

    return best_pattern


# 시간 측정
# Main
start_Time = time.time() 
    
if len(sys.argv) != 4:
    print("CMD 입력: python Assignment8.py input.txt output.txt and Finding_pattern_length")
    sys.exit()

input_file = sys.argv[1] #아큐먼트 입력 파일로 입력파일을 받는다. 
output_file = sys.argv[2] #아규먼트 커맨드 라인으로 출력결과를 저장하는 파일을 받는다
pattern_length = int(sys.argv[3])  # 아규먼트 커맨드 라인으로 패턴 찾기 길이를 받는다. 

try:
    sequences =  Fasta_R(input_file)
except FileNotFoundError:
    print("No input file") #파일이 없는 경우 
    sys.exit(1)

if not sequences:
    print("No DNA sequence") # DNA seq가 아닌 경우
    sys.exit(1)
elif len(sequences) == 1:
    print("Need more sequences") # 시퀀스가 부족한 경우 
    sys.exit(1)

# 시퀀스 DNA가 유효한지 확인을 한다. 
Valid_Bases = set("ATCG")

for sequence in sequences:
    if not set(sequence).issubset(Valid_Bases):
        print("No DNA sequence") #시퀀스가 아닌 경우 
        sys.exit(1)

patterns = Approximate_patterns_Finding(sequences, pattern_length) 
if not patterns:
    print("No pattern found") #패턴을 찾을 수 없는 경우 
    sys.exit(1)

try: 
    with open(output_file, "w") as f: #출력 결과를 파일 저장
        for pattern in patterns:
            f.write(pattern + "\n")
except Exception as error:
    print(error)
    sys.exit(1)

#시간 측정 끝    
end_Time = time.time()
    
# 측정한 시간을 뽑는다.
elapsed_Time = round((end_Time - start_Time) * 10**6)  # microseconds로 변환
print("Elapsed time (microseconds):", elapsed_Time) #걸린 시간 출력 