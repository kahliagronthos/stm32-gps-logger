import serial, csv, time, re, folium

PORT = '/dev/tty.usbserial-0001'  # confirm with: ls /dev/tty.*
BAUD = 115200
coords = []

with serial.Serial(PORT, BAUD, timeout=2) as ser, open('track.csv', 'w') as f:
    
    writer = csv.writer(f)
    writer.writerow(['timestamp', 'lat', 'lon', 'speed_kn'])
    
    while True:
        line = ser.readline().decode('utf-8', errors='ignore').strip()
        
        # STM32 format: UTC: 023541.00  Lat: 40.671341  Lon: -73.952660  Speed: 0.59 kts
        m = re.search(r'Lat:\s*([-\d.]+)\s+Lon:\s*([-\d.]+)\s+Speed:\s*([\d.]+)', line)
        if not m: continue
        lat, lon, spd = float(m[1]), float(m[2]), float(m[3])
        coords.append((lat, lon))
        writer.writerow([time.time(), lat, lon, spd])
        print(f"{lat:.6f}, {lon:.6f}  {spd} kts")
        
        # Rebuild map on every fix
        mp = folium.Map(location=coords[-1], zoom_start=17)
        folium.PolyLine(coords, color='red', weight=3).add_to(mp)
        mp.save('track.html')