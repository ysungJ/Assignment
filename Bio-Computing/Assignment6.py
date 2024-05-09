"""
Bio-Computing 
Assignment6 - Multiple Sequence Alignment

약 1만 5천개 프로틴 시퀀스 처리 시간  163.35초

<주의>
The input sequence is case-insensitive.
If the input file in command-line arguments does not exist, then print "No input file".
If the input file has nothing, then print "No protein sequence".
If the input file does not have protein sequences, then print "No protein sequences".
If the input file does not follow the FASTA format, then print "No correct format".
If the input file has only one protein sequence, then print "Need more sequences".
Your code should read all protein sequences in the input file.
Put "-" in the alignment for each gap.
Use the linear gap penalty for gaps.
During the computation of an optimal score for each cell in the 2-dimensional matrix, if the scores computed by two or more previous results are tied, prioritize the score from the previous result along the diagonal.
In each line, print the alignment up to 50 characters including gaps.
Coding competition: The fastest python code which runs correctly will have a 10% extra credit.
"""
import sys, time

#blousm62 행렬 정의 
#넘파이x 딕셔너리를 이용한 하드코딩 행열 정의
#Blosum62는 대칭 행렬임으로 겹치면 1번만 정의 ex) ('C','D') = ('D','C') = -3
blosum62_matrix = {
    ('C', 'C'): 9, ('C', 'S'): -1, ('C', 'T'): -1, ('C', 'P'): -3, ('C', 'A'): 0, ('C', 'G'): -3, ('C', 'N'): -3, ('C', 'D'): -3, ('C', 'E'): -4, ('C', 'Q'): -3, ('C', 'H'): -3, ('C', 'R'): -3, ('C', 'K'): -3, ('C', 'M'): -1, ('C', 'I'): -1, ('C', 'L'): -1, ('C', 'V'): -1, ('C', 'F'): -2, ('C', 'Y'): -2, ('C', 'W'): -2,
    ('S', 'S'): 4, ('S', 'T'): 1, ('S', 'P'): -1, ('S', 'A'): 1, ('S', 'G'): 0, ('S', 'N'): 1, ('S', 'D'): 0, ('S', 'E'): 0, ('S', 'Q'): 0, ('S', 'H'): -1, ('S', 'R'): -1, ('S', 'K'): 0, ('S', 'M'): -1, ('S', 'I'): -2, ('S', 'L'): -2, ('S', 'V'): -2, ('S', 'F'): -2, ('S', 'Y'): -2, ('S', 'W'): -3,
    ('T', 'T'): 4, ('T', 'P'): -1, ('T', 'A'): 0, ('T', 'G'): -2, ('T', 'N'): 0, ('T', 'D'): -1, ('T', 'E'): -1, ('T', 'Q'): -1, ('T', 'H'): -2, ('T', 'R'): -1, ('T', 'K'): -1, ('T', 'M'): -1, ('T', 'I'): -1, ('T', 'L'): -1, ('T', 'V'): 0, ('T', 'F'): -2, ('T', 'Y'): -2, ('T', 'W'): -2,
    ('P', 'P'): 7, ('P', 'A'): -1, ('P', 'G'): -2, ('P', 'N'): -2, ('P', 'D'): -1, ('P', 'E'): -1, ('P', 'Q'): -1, ('P', 'H'): -2, ('P', 'R'): -2, ('P', 'K'): -1, ('P', 'M'): -2, ('P', 'I'): -3, ('P', 'L'): -3, ('P', 'V'): -2, ('P', 'F'): -4, ('P', 'Y'): -3, ('P', 'W'): -4,
    ('A', 'A'): 4, ('A', 'G'): 0, ('A', 'N'): -2, ('A', 'D'): -2, ('A', 'E'): -1, ('A', 'Q'): -1, ('A', 'H'): -2, ('A', 'R'): -1, ('A', 'K'): -1, ('A', 'M'): -1, ('A', 'I'): -1, ('A', 'L'): -1, ('A', 'V'): 0, ('A', 'F'): -2, ('A', 'Y'): -2, ('A', 'W'): -3,
    ('G', 'G'): 6, ('G', 'N'): 0, ('G', 'D'): -1, ('G', 'E'): -2, ('G', 'Q'): -2, ('G', 'H'): -2, ('G', 'R'): -2, ('G', 'K'): -2, ('G', 'M'): -3, ('G', 'I'): -4, ('G', 'L'): -4, ('G', 'V'): -3, ('G', 'F'): -3, ('G', 'Y'): -3, ('G', 'W'): -2,
    ('N', 'N'): 6, ('N', 'D'): 1, ('N', 'E'): 0, ('N', 'Q'): 0, ('N', 'H'): 1, ('N', 'R'): 0, ('N', 'K'): 0, ('N', 'M'): -2, ('N', 'I'): -3, ('N', 'L'): -3, ('N', 'V'): -3, ('N', 'F'): -3, ('N', 'Y'): -2, ('N', 'W'): -4,
    ('D', 'D'): 6, ('D', 'E'): 2, ('D', 'Q'): 0, ('D', 'H'): -1, ('D', 'R'): -2, ('D', 'K'): -1, ('D', 'M'): -3, ('D', 'I'): -3, ('D', 'L'): -4, ('D', 'V'): -3, ('D', 'F'): -3, ('D', 'Y'): -3, ('D', 'W'): -4,
    ('E', 'E'): 5, ('E', 'Q'): 0, ('E', 'H'): -1, ('E', 'R'): -1, ('E', 'K'): 1, ('E', 'M'): -2, ('E', 'I'): -3, ('E', 'L'): -3, ('E', 'V'): -2, ('E', 'F'): -3, ('E', 'Y'): -2, ('E', 'W'): -3,
    ('Q', 'Q'): 5, ('Q', 'H'): 0, ('Q', 'R'): 1, ('Q', 'K'): 1, ('Q', 'M'): 0, ('Q', 'I'): -3, ('Q', 'L'): -2, ('Q', 'V'): -2, ('Q', 'F'): -3, ('Q', 'Y'): -1, ('Q', 'W'): -2,
    ('H', 'H'): 8, ('H', 'R'): 0, ('H', 'K'): -1, ('H', 'M'): -1, ('H', 'I'): -3, ('H', 'L'): -3, ('H', 'V'): -3, ('H', 'F'): -1, ('H', 'Y'): 2, ('H', 'W'): -2,
    ('R', 'R'): 5, ('R', 'K'): 2, ('R', 'M'): -1, ('R', 'I'): -3, ('R', 'L'): -2, ('R', 'V'): -3, ('R', 'F'): -3, ('R', 'Y'): -2, ('R', 'W'): -3,
    ('K', 'K'): 5, ('K', 'M'): 0, ('K', 'I'): -3, ('K', 'L'): -2, ('K', 'V'): -2, ('K', 'F'): -3, ('K', 'Y'): -2, ('K', 'W'): -3,
    ('M', 'M'): 5, ('M', 'I'): 2, ('M', 'L'): 2, ('M', 'V'): 1, ('M', 'F'): 0, ('M', 'Y'): 0, ('M', 'W'): -1,
    ('I', 'I'): 4, ('I', 'L'): 2, ('I', 'V'): 3, ('I', 'F'): 1, ('I', 'Y'): 0, ('I', 'W'): -1,
    ('L', 'L'): 4, ('L', 'V'): 1, ('L', 'F'): 2, ('L', 'Y'): 1, ('L', 'W'): -1,
    ('V', 'V'): 4, ('V', 'F'): 1, ('V', 'Y'): -1, ('V', 'W'): -3,
    ('F', 'F'): 6, ('F', 'Y'): 3, ('F', 'W'): 1,
    ('Y', 'Y'): 7, ('Y', 'W'): 2,
    ('W', 'W'): 11
}

#Fasta 형식을 읽는 함수  
def Fasta_R(Input_file):
    Sequences = []
    with open(Input_file, "r") as f:
        Sequence = ""
        for line in f:
            if line.startswith(">"):
                if Sequence:
                    Sequences.append(Sequence.upper()) #대소문자 구뷴
                    Sequence = ""
            else:
                Sequence += line.strip()
        if Sequence:
            Sequences.append(Sequence.upper()) # 대소문자 구분 

    return Sequences

#blosum62 매트릭스에서 aa1와 aa2의 점수를 가져오는 함수 
#blosum62는 대칭행렬이기 때문에 데이터의 순서를 구분하지않고 점수를 가지고온다. 
def blosum62_Score_Get(aa1, aa2, blosum62_matrix):
    return  blosum62_matrix.get((aa1, aa2), blosum62_matrix.get((aa2, aa1), -5))

#다이나믹 프로그래밍 알고리즘을 사용하여 시퀀스 쌍을 정렬하는 함수 
def Pairwise_Alignment(seq1, seq2, gap_penalty, blosum62_matrix):
    #점수 초기화
    m = len(seq1)
    n = len(seq2)
    Matrix_Score = [[0] * (n + 1) for _ in range(m + 1)]
    Traceback_Matrix = [[None] * (n + 1) for _ in range(m + 1)]
    
    for i in range(1, m + 1):
        Matrix_Score[i][0] = Matrix_Score[i-1][0] + gap_penalty
        Traceback_Matrix[i][0] = "up"
    for j in range(1, n + 1):
        Matrix_Score[0][j] = Matrix_Score[0][j-1] + gap_penalty
        Traceback_Matrix[0][j] = "left"
    
    # 시퀀스 스코어 항목을 계산하고 채운다. 
    for i in range(1, m + 1):
        for j in range(1, n + 1):
            match = Matrix_Score[i-1][j-1] + blosum62_Score_Get(seq1[i-1], seq2[j-1],  blosum62_matrix)
            delete = Matrix_Score[i-1][j] + gap_penalty
            insert = Matrix_Score[i][j-1] + gap_penalty
            Matrix_Score[i][j] = max(match, delete, insert)
            
            #최대 스코어를 기준으로 추적하는 행렬을 업데이트한다.
            if Matrix_Score[i][j] == match:
                Traceback_Matrix[i][j] = "diagonal"
            elif Matrix_Score[i][j] == delete:
                Traceback_Matrix[i][j] = "up"
            else:
                Traceback_Matrix[i][j] = "left"
    
    # 정렬을 찾기위해서 Traceback과정을 한다.
    alignment1 = ""
    alignment2 = ""
    i = m
    j = n

    while i > 0 and j > 0:
        if Traceback_Matrix[i][j] == "diagonal":
            alignment1 = seq1[i-1] + alignment1
            alignment2 = seq2[j-1] + alignment2
            i -= 1
            j -= 1
        elif Traceback_Matrix[i][j] == "up":
            alignment1 = seq1[i-1] + alignment1
            alignment2 = "-" + alignment2
            i -= 1
        else:
            alignment1 = "-" + alignment1
            alignment2 = seq2[j-1] + alignment2
            j -= 1
    while i > 0:
        alignment1 = seq1[i-1] + alignment1
        alignment2 = "-" + alignment2
        i -= 1
    while j > 0:
        alignment1 = "-" + alignment1
        alignment2 = seq2[j-1] + alignment2
        j -= 1
    
    return alignment2 

def Multiple_Alignment(sequences, gap_penalty, blosum62_matrix):
    # star alignment 휴라스틱 개념을 사용하여 다중 시퀀스 정렬 수행 
    Center_sequence = sequences[0]
    alignments = [Center_sequence]
    scores = [0] * len(sequences)
    
    # 중심이 되는 시퀀스를 찾는다 
    for i in range(1, len(sequences)):
        for j in range(len(Center_sequence)):
            for k in range(len(sequences[i])):
                scores[i] += blosum62_Score_Get(Center_sequence[j], sequences[i][k],blosum62_matrix)
    
    center_index = scores.index(max(scores))
    Center_sequence = sequences[center_index]
    
    # 시퀀스를 중심으로 유사성에 따라 정렬 수행 
    sequences.sort(key=lambda x: 
                   sum(blosum62_Score_Get(Center_sequence[j], x[k], blosum62_matrix) 
                       for j in range(len(Center_sequence)) for k in range(len(x))), reverse=True)

    # 시퀀스를 중심으로 Pairwise Align,ents와 정렬 수행 
    for i in range(1, len(sequences)):
        alignment2 = Pairwise_Alignment(Center_sequence, sequences[i], gap_penalty,  blosum62_matrix)
        
        aligned_sequence = ""
        for j in range(len(alignment2)): #2
            if alignment2[j] != "-": #2
                aligned_sequence += alignment2[j] #2
        alignments.append(aligned_sequence)
    
    # 매 갭마다 정렬에 "-" 추가
    aligned_sequences = list(zip(*alignments))
    for i in range(len(aligned_sequences)):
        if "-" in aligned_sequences[i]:
            for j in range(len(alignments)):
                alignments[j] = alignments[j][:i] + "-" + alignments[j][i:]
    
    return alignments

# 첫 번째 서열을 기준으로 모든 열에 대해 동일한 아미노산를 가지는지 확인하는 함수 
# 동일한 아미노산열이면 마지막 라인에 *를 추가
def Conservation_StarMark_Add(alignments):
    min_length = min(len(seq) for seq in alignments)
    for i in range(min_length):
        if all(alignment[i] == alignments[0][i] for alignment in alignments[1:]):
            for j in range(len(alignments)):
                alignments[j] = alignments[j][:i] + "*" + alignments[j][i+1:]
    return alignments

#시간측정
start_Time = time.time()

# MAIN 부분
if len(sys.argv) != 3:
    print("CMD 입력 : python Assignment6.py input.txt output.txt")

else:
    input_file = sys.argv[1]
    output_file = sys.argv[2]
    
    #파일이 존재하는지 확인한다. 
    try:
        with open(input_file, "r") as f:
            pass
    except FileNotFoundError:
        print("No input file")
        sys.exit(1)
    
    # 입력 파일로부터 시퀀스를 읽는다. 
    sequences = Fasta_R(input_file)
    
    # 입력파일에 프로틴 시퀀스가 있는지 확인 및 fasta format 형식 확인 
    if not sequences:
        print("No protein sequences")
        sys.exit(1)

    for seq in sequences:
        if not set(seq).issubset(set("ACDEFGHIKLMNPQRSTVWY")):  # FASTA 형식의 아미노산 코드를 확인
            print("No correct format")
            sys.exit(1)

    # 입력 파일에 둘 이상의 프로틴 시퀀스가 있는지 확인 
    if len(sequences) < 2:
        print("Need more sequences")
        sys.exit(1)

    # 다중 시퀀스 정렬 
    alignments = Multiple_Alignment(sequences, -5, blosum62_matrix)

    #"*"를 추가하는 함수
    Star_Alignments = Conservation_StarMark_Add(alignments) 
    
    #Alignment를 출력한 결과를 파일로 저장
    with open(output_file, "w") as f:
        for alignment in Star_Alignments:
            for i in range(0, len(alignment), 50):
                f.write(alignment[i:i+50] + "\n") #50단위마다 끊어서 출력한다.

    end_Time = time.time() # 종료 시간 측정
    elapsed_Time = round((end_Time - start_Time) * 10**6)  # microseconds로 변환
    print("Elapsed time (microseconds):", elapsed_Time)