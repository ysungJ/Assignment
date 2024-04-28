"""
바이오 컴퓨팅 과제 5 - Local Pairwise Sequence Alignment 
D-DAY 4/28
2019253084 전유성

+ 변수명 수정!
<요약> 
두 개의 DNA 서열 읽어와서 매칭 되면 +2점 미스 매칭 -2 갭은 -1점 사용하여 Local Pairwise
Sequence Alignment 구하고 화면에 스코어 점수 표시하기 

<주의> 
The input sequence is case-insensitive.
If the input file in command-line arguments does not exist, then print "No input file".
If the input file has nothing, then print "No DNA sequence".
If the input file contains more than two sequences, then use the first two sequences to implement pairwise sequence alignment.
If the input file contains less than two sequences, then print "Need more sequences".
If any of the first two sequences in the input file is not a DNA sequence, then print "No DNA sequence". A DNA sequence contains "A", "T", "C", and "G" only, and does not allow any special characters including a whitespace.
If any of the first two sequences in the input file does not follow the FASTA format, then print "No correct format".
Use the linear gap penalty for gaps.
Coding competition: The fastest python code which runs correctly will have a 10% extra credit.

"""
import sys, time

#Fasta 형식 읽는 함수 
def Fasta_R(Input_File):
    Sequences = []
    try:
        with open(Input_File, "r") as file:
            Current_seq = ''
            for line in file:
                line = line.strip().upper()  # 대소문자 구분 x
                if line.startswith(">"):
                    if Current_seq:  
                        Sequences.append(Current_seq)  
                    Current_seq = ''
                else:
                    if all(base in "ATCG" for base in line):  
                        Current_seq += line
                    else:
                        print("No DNA sequence")
                        sys.exit(1)
            if Current_seq:  
                Sequences.append(Current_seq)  
                if len(Sequences) < 2:
                    print("Need more sequences")
                    sys.exit(1)
            else:
                print("No DNA sequence")
                sys.exit(1)
    except FileNotFoundError:
        print("No input file")
        sys.exit(1)
    except Exception as error:
        print("An error occurred while reading the input file:", error)
        sys.exit(1)
        
    return Sequences[:2]  

def Local_Alignment(Seq1, Seq2, Match_score=2, Mismatch_score=-2, Gap_penalty=-1):
    m = len(Seq1)
    n = len(Seq2)

    # 스코어 매트릭스 초기화 
    Matrix_score = [[0] * (n + 1) for _ in range(m + 1)]

    Max_score = 0
    max_i = 0 
    max_j = 0

    #  매트릭스 채우기 및 최대 스코어 점수 찾기 
    for i in range(1, m + 1):
        for j in range(1, n + 1):
            if Seq1[i - 1] == Seq2[j - 1]:
                score = Matrix_score[i - 1][j - 1] + Match_score
            else:
                score = max(Matrix_score[i - 1][j] + Gap_penalty,
                            Matrix_score[i][j - 1] + Gap_penalty,
                            Matrix_score[i - 1][j - 1] + Mismatch_score,
                            0)

            Matrix_score[i][j] = score

            if score > Max_score:
                Max_score = score
                max_i = i
                max_j = j

    # 추적하여 정렬 찾기 
    Aligned_seq1 = ''
    Aligned_seq2 = ''
    i = max_i
    j = max_j
    while i > 0 and j > 0 and Matrix_score[i][j] != 0:
        if Matrix_score[i][j] == Matrix_score[i - 1][j - 1] + (Match_score if Seq1[i - 1] == Seq2[j - 1] else Mismatch_score):
            Aligned_seq1 = Seq1[i - 1] + Aligned_seq1
            Aligned_seq2 = Seq2[j - 1] + Aligned_seq2
            i -= 1
            j -= 1
        elif Matrix_score[i][j] == Matrix_score[i - 1][j] + Gap_penalty:
            Aligned_seq1 = Seq1[i - 1] + Aligned_seq1
            Aligned_seq2 = '-' + Aligned_seq2
            i -= 1
        else:
            Aligned_seq1 = '-' + Aligned_seq1
            Aligned_seq2 = Seq2[j - 1] + Aligned_seq2
            j -= 1

    return Aligned_seq1, Aligned_seq2, Max_score

#MAIN 부분
start_Time = time.time() #메인에서 시간을 측정해본다 

if len(sys.argv) != 3:
    print("CMD 입력: python Assignment5.py input.txt output.txt")
    sys.exit(1)

input_file = sys.argv[1]
output_file = sys.argv[2]

try:
    Seq = Fasta_R(input_file)
except Exception as error:
    print(error)
    sys.exit(1)

if len(Seq) < 2:
    print("Need more sequences")
    sys.exit(1)

aligned_seq1, aligned_seq2, similarity_score = Local_Alignment(Seq[0], Seq[1])

with open(output_file, "w") as file:
    file.write(aligned_seq1 + "\n")
    file.write(aligned_seq2 + "\n")

print("Similarity score:", similarity_score)

end_Time = time.time() #메인에서 측정한 시간을 끝난다. 
elapsed_Time = round((end_Time - start_Time) * 10**6)  # microseconds로 변환
print("Elapsed time (microseconds):", elapsed_Time)