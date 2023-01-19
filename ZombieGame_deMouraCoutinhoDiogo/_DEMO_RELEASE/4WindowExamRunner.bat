start GPP_TEST_RELEASE.exe -x 30 -y 30 -s 1000

start GPP_TEST_RELEASE.exe -x 1000 -y 30 -s 500

start GPP_TEST_RELEASE.exe -x 30 -y 540 -s 200

start GPP_TEST_RELEASE.exe -x 1000 -y 540 -s 1003
////36
echo new ActiveXObject("WScript.Shell").AppActivate("GPP_TEST_RELEASE.exe"); > tmp.js
cscript //nologo tmp.js & del tmp.js