"""
영상처리과제2-b
Perform Histogram Equalization and then plot the histogram
"""

import struct, numpy as np
import matplotlib.pyplot as plt
import matplotlib.image as img


# BMP 파일에서 헤더와 원시 데이터 추출
def get_bmp_data(filepath):
    bmpheader = list()
    raw_data = list()
    with open(filepath, 'rb') as bmp:
        bmpheader = struct.unpack('B' * 1078, bmp.read(1078))
        raw_data = struct.unpack('B' * 512 * 512, bmp.read(512 * 512))
    return bmpheader, raw_data

# 원시 데이터에서 픽셀 값의 빈도수를 세는 함수
def count_number(raw_data):
    count_num_list = [0 for i in range(256)]
    for i in raw_data:
        count_num_list[i] += 1
    return count_num_list

# 히스토그램을 그래프로 그리는 함수
def draw_graph(hist_data, img_file, output_path, mode=True):
    plt.figure(figsize=(16, 9))
    plt.subplot(1, 2, 1)
    if mode:
        output_path += '_bar.png'
        count_num_list = count_number(hist_data)
        x = np.arange(256)
        y = np.array(count_num_list)
        plt.bar(x, y)
    else:
        output_path += '_hist.png'
        plt.hist(hist_data, bins=255)

    plt.subplot(1, 2, 2)
    image = img.imread(img_file)
    plt.imshow(image, cmap='gray')
    plt.savefig(output_path, dpi=600)
    plt.show()

# 히스토그램 평활화 수행 함수
def hist_equalization(raw_data, count_num_list):
    LUT = [0 for i in range(len(count_num_list))]

    accumulate_sum = 0
    for i in range(len(count_num_list)):
        accumulate_sum += count_num_list[i]
        LUT[i] = int(255 * accumulate_sum / (512 * 512) + 0.5)
        
        if LUT[i] > 255:
            LUT[i] = 255
        elif LUT[i] < 0:
            LUT[i] = 0

    hist_data = [0 for i in range(len(raw_data))]
    for i in range(len(raw_data)):
        hist_data[i] = LUT[raw_data[i]]

    return hist_data

# 새로운 BMP 이미지를 생성하는 함수
def create_bmp_img(BMPHEADERS, raw_data, new_img_path):
    with open(new_img_path, 'wb') as bmp:
        for i in range(len(BMPHEADERS)):
            bmp.write(struct.pack('B', BMPHEADERS[i]))
        
        for i in range(len(raw_data)):
            bmp.write(struct.pack('B', raw_data[i]))

# 입력 파일 경로
input_file = 'lena_bmp_512x512_new.bmp'

# 출력 그래프 경로 및 새로운 이미지 경로
output_graph_path = 'outputs/problem_01_b'
new_img_path = 'outputs/b_equalization_lena.bmp'

# BMP 파일에서 데이터 추출
BMPHEADERS, raw_data = get_bmp_data(input_file)
# 원시 데이터의 픽셀 값 빈도수 계산
count_num_list = count_number(raw_data)
# 히스토그램 평활화 수행
hist_data = hist_equalization(raw_data, count_num_list)
# 새로운 BMP 이미지 생성
create_bmp_img(BMPHEADERS, hist_data, new_img_path)
# 히스토그램 그래프 그리기
draw_graph(hist_data, new_img_path, output_graph_path, Tru