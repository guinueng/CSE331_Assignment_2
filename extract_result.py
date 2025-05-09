import os
import re
import csv

# 설정: 특정 단어와 경로
filename_keyword = "1m"  # 파일 이름에 포함된 특정 단어
# content_keyword = "Sorting time: "  # 파일 내용에서 찾고자 하는 특정 단어
content_keyword = "Maximum resident set size (kbytes): "  # 파일 내용에서 찾고자 하는 특정 단어
input_folder = "./"  # txt 파일이 있는 폴더 경로
output_csv = "quick_1m_mem.csv"  # 결과를 저장할 CSV 파일 이름

# 모든 숫자를 저장할 딕셔너리 (파일별로 열 생성)
data = {}

# 특정 단어가 파일 이름에 포함된 파일만 처리
for filename in os.listdir(input_folder):
    if filename_keyword in filename and filename.endswith(".txt"):  # 파일 이름 필터링
        file_path = os.path.join(input_folder, filename)
        print(f"'{filename}' 파일을 처리 중입니다...")
        
        # 현재 파일에서 숫자를 저장할 리스트
        current_numbers = []
        
        with open(file_path, 'r', encoding='utf-8') as file:
            for line in file:
                if content_keyword in line:  # 내용에서 특정 단어 검색
                    found_numbers = re.findall(r'\d+(?:\.\d+)?', line)  # 정수와 소수 추출
                    current_numbers.extend(map(float, found_numbers))  # 숫자를 리스트에 추가
        
        # 딕셔너리에 현재 파일의 숫자 추가 (파일명을 키로 사용)
        data[filename] = current_numbers

# CSV 파일에 데이터 저장 (파일별로 열 생성)
with open(output_csv, 'w', newline='', encoding='utf-8') as csv_file:
    writer = csv.writer(csv_file)
    
    # 헤더 작성 (파일 이름을 열 제목으로 사용)
    header = list(data.keys())
    writer.writerow(header)
    
    # 데이터 작성 (행 단위로 작성)
    max_length = max(len(numbers) for numbers in data.values())  # 가장 긴 숫자 리스트 길이 계산
    for i in range(max_length):
        row = [data[file][i] if i < len(data[file]) else "" for file in header]
        writer.writerow(row)

# 평균 계산 및 출력
for filename, numbers in data.items():
    if numbers:
        average = sum(numbers) / len(numbers)
        print(f"'{filename}'에서 추출된 숫자의 평균: {average}")
    else:
        print(f"'{filename}'에서 추출된 숫자가 없습니다.")