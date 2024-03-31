"""
바이오 컴퓨팅 과제 1

<과제 요약>
DNA시퀀스을 역순으로 출력하는 프로그램을 작성 ATCG는 DNA 시퀀스이다.
EX) ACCTGG -> GGTCCA (reverse) -> CCAGGT (complement)

메모장의 주석은 >이며, >의 다음줄부터 시퀀스가 시작한다.
> 설명 or 시퀀스 이름일수도 있다.
acatGAcgtc
aCTGga
>설명 설명~  
위에가 한 세트의 시퀀스이다.

<주의>
1. 대소문자를 구분하지않는다. 
2. DNA의 시퀀스가 없는 경우 "No DNA Sequence"가 출력되어야한다.
3. 커맨드 라인으로 입력 파일은 in.txt, 
출력파일은 out.txt가 나오게 해야한다. ex) python Assignment1.py in.txt out.txt
"""
#필요한 모듈을 라이브러리에서 불러온다.
import sys

#DNA시퀀스를 reverse 해주는 함수 선언
def reverseSeq(seq):
    return seq[::-1] #역순으로 반환

#reverse한 함수를 Complement 해주는 함수 선언 
def complementSeq(seq):
    #매핑해주는 딕셔너리를 선언 
    complement = {'A': 'T', 'T': 'A', 'C': 'G', 'G': 'C'}
    #이를 문자열로 반환 해준다. 
    complementSeq = ''.join(complement.get(base, base) for base in seq)
    return complementSeq #반환 

#dan 시퀀스를 읽는다.
def read_DNAseq(inputF):
    DNA_seq = [] #리스트 선언 

    # input 파일을 열고 처리한다. 
    with open(inputF, 'r') as f:
        dnaSeq = ""
        for line in f:
            line = line.strip() # 줄 바꿈 문자를 제거한다.
            if line.startswith(">"): # >로 시작하는 줄은 주석, 동시에 시퀀스에 대한 설명
                if dnaSeq: #기존 시퀀스가 있으면 리스트에 추가
                    DNA_seq.append(dnaSeq.upper()) #대소문자 구분
                    dnaSeq = ""  #다음 시퀀스를 초기화
            else: #시퀀스가 데이터일 경우 
                dnaSeq += line #데이터를 이어붙인다.
        if dnaSeq: #마지막 시퀀스가 있는지 검사
            DNA_seq.append(dnaSeq.upper()) #대소문자 구분
     
    if not set(DNA_seq).issubset({'A', 'C', 'G', 'T'}): #메모장에 DNA시퀀스가 없는 경우
     print("NO DNA SEQUENCE")
     sys.exit(1) # 프로그램 종료 

    return DNA_seq #반환

#메인 역할을 하는 코드 부분 이면서 Command Line Argument를 받는 부분 
if len(sys.argv) != 3: 
    print("CMD에 python Assignment1.py <입력 파일> <출력 파일>을 입력해주세요 ")
    print( "EX) python Assignment1.py in.txt out.txt 입력 ")
    sys.exit(1) #프로그램 종료 

#입력 파일명은 1번 인자로 받는다. 
inputF = sys.argv[1] 
outputF = sys.argv[2] 
#입력을 했을 경우 
try:
    dnaSequences = read_DNAseq(inputF) #입력한 파일에서 시퀀스 데이터 읽기
   
      #파일을 읽을 수 없는 경우  (파일자체가 오류일 경우)
    if not dnaSequences: 
        print("DNA sequence의 데이터가 없습니다.")
        sys.exit(1) #프로그램 종료 

    # Reverse-complement를 한 DNA시퀀스를 out파일로 만든다. 
    with open(outputF, 'w') as out_file: #파일명을 out.txt로 정한다. 
        for dnaSequence in dnaSequences:
            reverse_Seq = reverseSeq(dnaSequence) # 시퀀스를 리버스
            reverse_Com_seq = complementSeq(reverse_Seq) #리버스한 시퀀스를 컴플먼트 
            out_file.write(reverse_Com_seq + '\n') #파일에 결과를 쓴다. 

    print("출력 파일에 Reverse_Complement된 DNA시퀀스를 출력하였습니다.")

#파일을 찾을 수 없는 경우 
except FileNotFoundError:
    print(f"에러 발생, 파일 '{inputF}' 찾을 수 없습니다.")
    sys.exit(1)# 프로그램 종료