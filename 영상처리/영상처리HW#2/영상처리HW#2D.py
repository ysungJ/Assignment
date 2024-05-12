"""
영상처리과제2-d
Perform Ends-in Contrast Stretching with low-end value of 30 and
high-end value of 200, and then plot the histogram. Discuss any
difference when compared to the result of (c).
"""

import struct
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.image as img

# BMP 파일에서 헤더와 원시 데이터를 추출하는 함수
def get_bmp_data(filepath):
    bmpheader = list()
    raw_data = list()
    with open(filepath, 'rb') as bmp:
        # BMP 헤더 정보를 읽어옴
        bmpheader = struct.unpack('B' * 1078, bmp.read(1078))
        # 이미지의 원시 데이터를 읽어옴
        raw_data = struct.unpack('B' * 512 * 512, bmp.read(512 * 512))
    return bmpheader, raw_data

# 원시 데이터에서 각 픽셀 값의 빈도수를 계산하는 함수
def count_number(raw_data):
    count_num_list = [0 for i in range(256)]
    for i in raw_data:
        count_num_list[i] += 1
    return count_num_list

# 변환된 이미지를 BMP 파일로 저장하는 함수
def create_bmp_img(BMPHEADERS, raw_data, new_img_path):
    with open(new_img_path, 'wb') as bmp:
        # BMP 헤더를 파일에 쓰기
        for i in range(len(BMPHEADERS)):
            bmp.write(struct.pack('B', BMPHEADERS[i]))
        # 변환된 이미지 데이터를 파일에 쓰기
        for i in range(len(raw_data)):
            bmp.write(struct.pack('B', raw_data[i]))

# 히스토그램과 이미지를 그래프로 그리는 함수
def draw_graph(hist_data, img_file, output_path, mode=True):
    plt.figure(figsize=(16, 9))
    plt.subplot(1, 2, 1)
    if mode:
        # 히스토그램 그래프를 생성하여 저장
        output_path += '_bar.png'
        count_num_list = count_number(hist_data)
        x = np.arange(256)
        y = np.array(count_num_list)
        plt.bar(x, y)
    else:
        # 히스토그램을 플롯하여 저장
        output_path += '_hist.png'
        plt.hist(hist_data, bins=255)

    plt.subplot(1, 2, 2)
    # 이미지를 그레이스케일로 읽어옴
    image = img.imread(img_file)
    plt.imshow(image, cmap='gray')
    # 그래프 저장
    plt.savefig(output_path, dpi=600)
    plt.show()

# Ends-in 대비 스트레칭을 수행하는 함수
def endsin_contrast_stretching(raw_data):
    hist_data = [0 for i in range(len(raw_data))]
    min_val = 30  # low-end 값
    max_val = 200  # high-end 값

    # Ends-in 대비 스트레칭 적용
    for i in range(len(raw_data)):
        hist_data[i] = int(255 * (raw_data[i] - min_val) / (max_val - min_val) + 0.5)
        if hist_data[i] > 255:
            hist_data[i] = 255
        elif hist_data[i] < 0:
            hist_data[i] = 0
    
    return hist_data

# 입력 및 출력 경로
input_file = 'lena_bmp_512x512_new.bmp'
output_graph_path = 'outputs/problem_01_d'
new_img_path = 'outputs/d_endsin_contrast_stretching_lena.bmp'

# BMP 파일에서 데이터 추출
BMPHEADERS, raw_data = get_bmp_data(input_file)

# Ends-in 대비 스트레칭 수행
hist_data = endsin_contrast_stretching(raw_data)

# 변환된 이미지 저장 및 그래프 그리기
create_bmp_img(BMPHEADERS, hist_data, new_img_path)
draw_graph(hist_data, new_img_path, output_graph_path, True)