#Auto Failed login attempt to trigger alert at Wazuh SIEM
for ($i=0; $i -lt 1000; $i++) {
    Start-Sleep -Milliseconds 200
    $null = (net use \\localhost\IPC$ /user:fakeuser wrongpass 2>&1)
}