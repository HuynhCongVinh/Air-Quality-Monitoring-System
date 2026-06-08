import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.ensemble import RandomForestRegressor
import joblib

# 1. Đọc dữ liệu từ file CSV
df = pd.read_csv('air_quality_data.csv')

# 2. Tiền xử lý: Tạo 2 nhãn mục tiêu cho tương lai (dòng tiếp theo)
df['target_pm25'] = df['pm2.5'].shift(-1)
df['target_eco2'] = df['eco2'].shift(-1)

# Loại bỏ dòng cuối cùng vì không có giá trị tương lai
df = df.dropna()

# 3. Chọn các tính năng đầu vào (Features)
X = df[['temp', 'hum', 'eco2', 'tvoc', 'pm1.0', 'pm2.5', 'pm10']]

# Chọn 2 đầu ra (Target) cùng lúc
y = df[['target_pm25', 'target_eco2']]

# 4. Chia dữ liệu
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)

# 5. Huấn luyện mô hình đa đầu ra
print("Đang huấn luyện bộ não AI dự báo đa chỉ số (PM2.5 & eCO2)...")
model = RandomForestRegressor(n_estimators=100, random_state=42)
model.fit(X_train, y_train)

# 6. Lưu mô hình
joblib.dump(model, 'air_quality_multi_model.pkl')
print("Đã lưu mô hình mới: 'air_quality_multi_model.pkl'")

# Kiểm tra độ chính xác
score = model.score(X_test, y_test)
print(f"Độ chính xác tổng hợp của mô hình: {score * 100:.2f}%")