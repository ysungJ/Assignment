"""
    바이오 컴퓨팅 과제 2 - GC Content

<과제 요약>
가장 높은 GC 함량을 가진 DNA 서열은 반환하기 

1. 여러개의 DNA 서열 일고 "G"와 "C"의 비율을 계산
2. "G"와 "C"을 비율을 가진 DNA 서열을 출력 파일에 출력 
3. CMD에는 "G"와 "C"의 비율이 얼마인지 나와야함.
4. 코드는 커맨드아규먼트라인으로 입출력 받을 것 

<주의> 

입력 서열은 대소문자 구분 X
입력 파일에 없으면 "NO DNA Sequence"출력
DNA서열이(ACTG) 아닌 다른 서열이 하나라도 있어도  "NO DNA Sequence"출력
"""
#필요한 라이브러리를 불러온다. 
import sys

#GC함량을 계산하는 함수 
def GC_cal(DNA_seq):
    GC_count = DNA_seq.upper().count("G") + DNA_seq.upper().count("C")
    return GC_count / len(DNA_seq)

#파스타 형식의 파일을 입력하여 DNA 서열을 사전에 반환하는 함수 
def Fasta_f(input_file):
   
    dna_seq = {}
    current_seq = ''
    description = ''
    
    with open(input_file, "r") as file:
        for line in file:
            line = line.strip()
            if line.startswith(">"):
                if current_seq:
                    dna_seq[description] = current_seq
                description = line[1:]
                current_seq = ''
            else:
               current_seq += line
     
     #마지막 서열 추가
        if current_seq:
            dna_seq[description] = current_seq
    
    return dna_seq

#커맨드 아규먼트 라인로 입력파일. 출력파일을 입력한다.
if len(sys.argv) != 3:
    print("python Assignment2.py input_file_name output_file_name를 쓰시오.")
    sys.exit(1)

input_file = sys.argv[1]
output_file = sys.argv[2]

#입력파일 부터 DNA 서열 확인 
dna_seq_f = Fasta_f(input_file)

# 파일이 존재 하지 않을 경우
if not dna_seq_f:
    print("파일이 열리지 않습니다. 다시한번 확인해보세요 ") 
    sys.exit(1)

#변수선언
GC_MAXcount = 0
select_seq = ''
select_description = ''

# GC 비율이 가장 높은 DNA를 찾기 
for description, seq in dna_seq_f.items():

# DNA 서열이 A, C, T, G 이외의 문자를 포함하는지 확인 
# 다른 문자가 있는 경우 그것은 DNA 서열이 아니기 때문에 종료한다. 
    if not all(base in "ACGT" for base in seq.upper()):
        print("NO DNA sequence")
        sys.exit(1)

# GC서열 비교
    gc_content = GC_cal(seq)
    if gc_content > GC_MAXcount:
        GC_MAXcount = gc_content
        select_seq = seq
        select_description = description

#화면에 GC 함량 출력 
print("GC 함량의 최고 비율 : ", round(GC_MAXcount,5)) #소수점 5번쨰 자리까지 출력.

#  그렇게 출력된 DNA을 파일에 쓴다.
with open(output_file, "w") as file:
    file.write(f">{select_description}\n{select_seq}")

print("파일이 출력 되었습니다. 확인해주세요", output_file)