"""
바이오 컴퓨팅 과제 9 - Character-Based Evolutionary Tree Reconstruction
<note>
The input sequence is case-insensitive.
If the input file in a command-line argument does not exist, then print "No input file".
If the input file has nothing, then print "No DNA sequence".
If any sequence in the input file is not a DNA sequence, then print "No DNA sequence".
If any sequence in the input file does not follow the fasta format, then print "No correct format".
If the input file does not contain 2^n sequences, then print "Incorrect number of sequences".
Please print the sequence of each node per line.
Coding competition: The fastest python code which runs correctly will have a 10% extra credit.

"""
import sys, os, time, re

# FASTA 형식 파일에서 서열을 읽는 함수
def Fasta_R(input_file):
    sequences = []
    with open(input_file, "r") as file:
        lines = file.read().strip().split("\n")
        if not lines:
            print("No DNA sequence")
            sys.exit(1)
        header = None
        seq = ''
        for line in lines:
            if line.startswith(">"):
                if header:
                    sequences.append(seq)
                header = line
                seq = ''
            else:
                if not header:
                    print("No correct format")
                    sys.exit(1)
                seq += line.upper().strip()
        if header:
            sequences.append(seq)
    return sequences

# 숫자가 2의 제곱수인지 확인하는 함수
def Two_Square(n):
    return (n != 0) and (n & (n-1)) == 0

# 서열이 DNA 서열인지 확인하는 함수
def Validate_Seq(sequences):
    if not sequences:
        print("No DNA sequence")
        sys.exit(1)
    for seq in sequences:
        if not re.match("^[ACTG]+$", seq):
            print("No DNA sequence")
            sys.exit(1)

# 모든 서열이 같은 길이인지 확인하는 함수
def Validate_Length(sequences):
    length = len(sequences[0])
    for seq in sequences:
        if len(seq) != length:
            print("Incorrect length of sequences")
            sys.exit(1)

# 두 집합을 병합하는 함수
def Merge_sort(a, b):
    return a & b if a & b else a | b

# Fitch 알고리즘을 사용하여 진화 트리를 구성하는 함수
def Tree_Construct(sequences):
    computed_nodes = {}

    # Fitch 알고리즘을 실행하는 함수
    def Fitch_Algorithm(seqs):
        if tuple(seqs) in computed_nodes:
            return computed_nodes[tuple(seqs)]
        nodes = [(set(seq), seq) for seq in seqs]
        while len(nodes) > 1:
            next_nodes = []
            for i in range(0, len(nodes), 2):
                set_a, seq_a = nodes[i]
                set_b, seq_b = nodes[i + 1]
                merged_set = Merge_sort(set_a, set_b)
                next_nodes.append((merged_set, (nodes[i], nodes[i + 1])))
            nodes = next_nodes
        computed_nodes[tuple(seqs)] = nodes[0]
        return nodes[0]

    # 진화 트리를 재구성하는 함수
    def Tree_Reconstruct(node):
        if isinstance(node[1], str):
            return node[1]
        left = Tree_Reconstruct(node[1][0])
        right = Tree_Reconstruct(node[1][1])
        node_seq = ''.join([Merge_sort(set(left[i]), set(right[i])).pop() for i in range(len(left))])
        return node_seq

    root = Fitch_Algorithm(sequences)
    root_seq = Tree_Reconstruct(root)
    parsimony_score = sum(1 if set(seq[i]) != set(root_seq[i]) else 0 for seq in sequences for i in range(len(seq)))
    return parsimony_score, root

# 메인 부분
# 시간 측정
start_Time = time.time()

if len(sys.argv) != 3:
    print("CMD 입력 : python Assignment9.py input.txt output.txt")
    sys.exit(1)

input_file = sys.argv[1]
output_file = sys.argv[2]

if not os.path.exists(input_file):
    print("No input file")
    sys.exit(1)

sequences = Fasta_R(input_file)

Validate_Seq(sequences)
Validate_Length(sequences)

if not Two_Square(len(sequences)):
    print("Incorrect number of sequences")
    sys.exit(1)

parsimony_score, root = Tree_Construct(sequences)

# 시간 측정 끝
end_Time = time.time()

with open(output_file, "w") as file:
    def write_tree(node):
        if isinstance(node[1], str):  # 노드가 문자열인 경우
            file.write(node[1] + "\n")
            return node[1]
        left = write_tree(node[1][0])  # 왼쪽 자식 노드로 재귀 호출
        right = write_tree(node[1][1])  # 오른쪽 자식 노드로 재귀 호출
        node_seq = ''.join([Merge_sort(set(left[i]), set(right[i])).pop() for i in range(len(left))])
        file.write(node_seq + "\n")
        return node_seq

    write_tree(root)

elapsed_Time = round((end_Time - start_Time) * 10**6)  # microseconds로 변환

print("Parsimony score:", parsimony_score)
print(f"Elapsed time: {elapsed_Time} microseconds")