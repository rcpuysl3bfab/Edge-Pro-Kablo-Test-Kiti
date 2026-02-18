@echo off
chcp 65001 >nul
cd /d "%~dp0"
if not exist .git git init
git add .
git status
git commit -m "Initial commit: Edge Pro Kablo Test Kiti" 2>nul || echo Nothing to commit or already committed
git branch -M main
git remote remove origin 2>nul
git remote add origin https://github.com/rcpuysl3bfab/Edge-Pro-Kablo-Test-Kiti.git
git pull origin main --allow-unrelated-histories 2>nul || echo No pull needed
git push -u origin main
pause
