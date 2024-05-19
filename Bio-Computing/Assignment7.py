"""
Bio-Computing - Assignment7 Pattern Finding

2019253084 전유성

The input sequence is case-insensitive.
If the input file in a command-line argument does not exist, then print "No input file".
If the input file has nothing, then print "No DNA sequence".
If the input file contains a single sequence, then print "Need more sequences".
If any sequence in the input file is not a DNA sequence, then print "No DNA sequence".
If any sequence in the input file does not follow the fasta format, then print "No correct format".
If multiple patterns of length m are found, then print all the patterns (each pattern per line).
If no pattern of length m is found, then print "No pattern found".
Coding competition: The fastest python code which runs correctly will have a 10% extra credit.

"""
import sys, time

# Fasta 형식을 읽는 함수
def Fasta_R(Input_file):
    Sequences = []
    with open(Input_file, "r") as f:
        Sequence = ""
        for line in f:
            if line.startswith(">"):
                if Sequence:
                    Sequences.append(Sequence.upper())  # 대소문자 구별
                    Sequence = ""
            else:
                Sequence += line.strip()  # 줄바꿈 문자를 제거하고 추가
        if Sequence:
            Sequences.append(Sequence)  # 대소문자 구별 및 마지막 서열을 리스트에 추가

    return Sequences

# 패턴 찾기하는 함수 
def Find_Patterns(sequences, m):
    patterns = set()
    for i in range(len(sequences[0]) - m + 1):  # 1번째 시퀀스을 기준으로 패턴을 찾는다 
        pattern = sequences[0][i:i+m]  # 패턴 뽑음 
        found_in_all = True
        for Sequence in sequences[1:]:  # 나머지 시퀀스에서 패턴이 있는지 없는지 확인 
            if pattern not in Sequence:  # 시퀀스에 패턴이 없을 경우 
                found_in_all = False
                break
        if found_in_all:  # 시퀀스에 패턴이 있는 경우 
            patterns.add(pattern)  # 패턴을 추가 
    return patterns

# 시간 측정
start_Time = time.time() 
    
if len(sys.argv) != 4:
    print("CMD 입력: python Assignment7.py input.txt output.txt and Finding_pattern_length")
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

patterns =Find_Patterns(sequences, pattern_length) 
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