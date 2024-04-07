"""
바이오컴퓨팅 과제 3

문제 - 반복되는 시퀀스를 찾기. 

1. 반복되는 세그먼트 크기는 2~4사이 (2,3,4)의 시퀸스 세그먼트가 
최소 2번이상 반복될 때이다 gaactgactgag or gcatatca or tagctgctac
2. 입력 파일에서 코드를 읽고 해당 시퀀스에서 가장 긴 반복 세그먼트를 출력 파일에 출력할 것 
3. 실행시간을 ms(마이크로초)단위로 화면에 출력할 것 
4. 커맨드 아규먼트 라인으로 입출력 파일 받을 것 
5. fasta형식의 dna서열이 있음. 

<노트>
대소문자 구분 x
입력 파일 내용이 비어있거나 시퀀스가 아니면 "No DNA SEQ"출력
파일안 내용에 여러 염기 서열이 있으면, 1번 서열을 사용
1번 서열이 dna 시퀀스가 아닌 경우에도 "No DNA seq"출력 
입력파일이 fasta형식이 아니면 "no correct format"출력 
반복된 세그먼트가 없는 경우 "No repeats"를 출력
동일한 길이의 여러 반복 세그먼트가 있으면 처음 발생한 세그먼트로 출력 
세그먼트는 중복될 수 있다 
시간복잡도를 이용해서 효율있게 코드를 짜면 (실행시간이 짧을 경우) 가산점이 존재

"""

#필요한 라이브러리 선언 
import sys, time
#시간이 얼마나 걸리는지 확인하기위해 선언 
time_Start= time.time()  

#함수 선언하는 부분 
# FASTA형식을 읽어주는 함수 선언 
def Fasta_R(input_F):
    try:
        with open(input_F, "r") as file:
            lines = file.readlines()
            Seq = ''
            for line in lines:
                #주석 처리 첫줄은 스킵한다.
                if line.startswith(">"): 
                    continue
                Seq += line.strip().upper()
            return Seq
    #파일을 찾을 수 없는 경우 
    except FileNotFoundError:
        print("파일을 찾을 수 없습니다. 프로그램을 종료합니다.")
        sys.exit(1)

#가장 길게 반복된 시퀀스를 찾는 함수 선언
def Long_Repeat_Segment(SEQ):
    n = len(SEQ)
    repeat = []
    # 세그먼트의 길이가 2,3,4까지 반복되는 경우에 
    for length in range(2, 5):
        #각 세그먼트를 확인하여 반복되는 세그먼트를 찾는데
        for i in range(n - length * 2 + 1):
            segment = SEQ[i:i + length]
            #다른 위치에서 같은 세그먼트를 찾으면 반복 리스트에 추가한다.
            for j in range(i + length, n - length + 1):
                if SEQ[j:j + length] == segment:
                    repeat.append(segment)
    #가장 길게 반복된 시퀀스가 있으면 반환
    if repeat:
        return max(repeat, key=len)
    else:
        #없는 경우에 반복이 없다고 반환
        return "No repeats" 
#커맨드  아규먼트 라인을 이용하여 입력파일 출력파일을 입력한다.
if len(sys.argv) != 3:
    print("사용법: python Assignment3.py input_file output_file을 입력하세요!")
    sys.exit(1)


#메인 역할하는 부분
input_file = sys.argv[1]
output_file = sys.argv[2]

#파일의 시퀀스를 읽는다. 
Main_seq = Fasta_R(input_file)
# 파일에 DNA 시퀀스가 존재하지않는경우 
if not Main_seq:
    print("No correct format")
    sys.exit(1)

#읽었는데 DNA서열이 아닌 경우 
if not all(base in "ATCG" for base in Main_seq):
    print("No DNA Sequence")
    sys.exit(1)

#가장 긴 반복된 시퀀스를 찾았을 경우
repeat_Segment = Long_Repeat_Segment(Main_seq)
#반복이 없을 경우 
if repeat_Segment == "No repeats":
    print(repeat_Segment)

else:
    #출력 파일에 결과를 쓴다.
    with open(output_file, 'w') as file:
        file.write(repeat_Segment)
    print(f"가장 긴 반복 세그먼트: {repeat_Segment} 입니다.")

time_End = time.time()  #종료 시간 기록
time_ms = round((time_End - time_Start) * 10**6) #실행시간 계산
print(f"경과 시간: {time_ms} 마이크로초")
