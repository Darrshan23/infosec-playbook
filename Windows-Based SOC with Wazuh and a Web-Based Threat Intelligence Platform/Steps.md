# Wazuh Lab Setup Guide - Windows WSL2

A complete guide to deploying a Wazuh SIEM lab environment on Windows using WSL2, Docker, and Sysmon for security monitoring and threat detection.

## Overview

This guide will walk you through:

- Preparing Windows (WSL2 + Docker Desktop)
- Deploying Wazuh server stack using Docker Compose
- Installing Wazuh agent on Windows
- Installing Sysmon with community configurations
- Generating test telemetry with Atomic Red Team
- Verifying logs and agent visibility in Wazuh dashboard

---

## Prerequisites

Before starting, ensure you have:

- **Windows 10 (version 2004+) or Windows 11** with WSL2 support
- **Administrator privileges** on your machine
- **Virtualization enabled** in BIOS/UEFI

### Verify Virtualization Status

Open PowerShell as Administrator and run:

```powershell
systeminfo | Select-String -Pattern "Hyper-V Requirements","Virtualization"
```

If virtualization is disabled, enter your BIOS/UEFI settings and enable it.

---

## Step 1: Enable WSL2 and Install Ubuntu

Open PowerShell as Administrator and execute:

```powershell
# Enable WSL and Virtual Machine Platform
dism.exe /online /enable-feature /featurename:VirtualMachinePlatform /all /norestart
dism.exe /online /enable-feature /featurename:Microsoft-Windows-Subsystem-Linux /all /norestart

# Set WSL default to version 2
wsl --set-default-version 2

# Install Ubuntu
wsl --install -d Ubuntu
```

**Note:** If `wsl --install` fails, install Ubuntu from the Microsoft Store, then launch it and set up your username and password.

### Update Ubuntu

Once inside the Ubuntu shell:

```bash
sudo apt update && sudo apt upgrade -y
sudo apt install -y curl git wget apt-transport-https ca-certificates unzip
```

---

## Step 2: Install Docker Desktop

Docker simplifies the Wazuh server deployment process.

1. Download **Docker Desktop for Windows** from [Docker's official site](https://www.docker.com/products/docker-desktop)
2. During installation, enable **WSL2 integration**
3. After installation, open Docker Desktop settings
4. Navigate to **Settings → Resources → WSL Integration**
5. Ensure your Ubuntu distribution is checked/enabled

### Verify Docker Installation

From your Ubuntu WSL terminal:

```bash
docker version
docker info
```

If Docker isn't responding, check the WSL integration settings in Docker Desktop.

---

## Step 3: Deploy Wazuh Server with Docker Compose

Wazuh provides a Docker Compose setup that includes the Manager, Elasticsearch/OpenSearch, and Kibana/Wazuh dashboard.

### Clone and Deploy

In your WSL Ubuntu terminal:

```bash
# Clone the Wazuh Docker repository
git clone https://github.com/wazuh/wazuh-docker.git
cd wazuh-docker

# Start the Wazuh stack
docker-compose -f docker-compose.yml up -d
```

### Monitor Deployment

Watch the container logs:

```bash
docker-compose -f docker-compose.yml logs -f
```

**Expected behavior:** Multiple containers will start (Wazuh Manager, Elasticsearch/OpenSearch, Kibana/Wazuh plugin).

**Troubleshooting:** If containers fail to start, check that Docker Desktop has sufficient resources allocated (minimum 4GB RAM, preferably 8GB). Adjust in Docker Desktop → Settings → Resources.

---

## Step 4: Access the Wazuh Dashboard

Open your web browser and navigate to the Wazuh dashboard:

```
https://localhost:5601
```

**Note:** The exact port depends on your docker-compose configuration. Check the repository README or container logs for default credentials.

Once logged in, navigate to **Wazuh app → Agents** to verify the manager is running (agents will appear once registered).

---

## Step 5: Install Wazuh Agent on Windows

### Download the Agent

Download the Wazuh agent MSI installer for Windows from the [official Wazuh website](https://wazuh.com). Match the agent version with your server version when possible.

### Install via GUI

Run the MSI installer as Administrator. During installation, set the **Manager IP** to your Wazuh server address:

- If running Docker on the same machine, use your Windows host IP (e.g., `127.0.0.1` or `host.docker.internal`)
- For WSL networking, you may need to use the WSL IP

### Install via Command Line

Alternatively, install from an elevated PowerShell:

```powershell
msiexec /i "wazuh-agent-4.x.x.msi" /quiet WAZUH_MANAGER="192.168.1.100"
```

Replace the IP with your actual Wazuh manager address.

### Start the Agent Service

```powershell
# Start the service
Start-Service -Name wazuh-agent

# Or restart if already running
Restart-Service -Name wazuh-agent

# Verify status
Get-Service -Name wazuh-agent
```

### Verify in Dashboard

Return to the Wazuh dashboard. You should see a new agent listed (possibly in "pending" status). Accept or register the agent if necessary through the dashboard interface.

---

## Step 6: Install Sysmon with Recommended Configuration

Sysmon provides detailed telemetry for security monitoring.

### Download Sysmon and Configuration

1. Download **Sysmon** from [Microsoft Sysinternals](https://docs.microsoft.com/en-us/sysinternals/downloads/sysmon)
2. Download a recommended configuration:
   - [SwiftOnSecurity's Sysmon Config](https://github.com/SwiftOnSecurity/sysmon-config) (popular choice)
   - [Olaf's Sysmon Config](https://github.com/olafhartong/sysmon-modular) (modular approach)

### Install Sysmon

From an elevated PowerShell:

```powershell
# Create temporary directory
New-Item -Path C:\Temp\sysmon -ItemType Directory -Force

# Place Sysmon.exe, sysmon64.exe, and your chosen config XML in C:\Temp\sysmon

# Install Sysmon with configuration (64-bit)
C:\Temp\sysmon\sysmon64.exe -i C:\Temp\sysmon\sysmon-config.xml
```

### Verify Sysmon Installation

```powershell
# Check service status
Get-Service -Name Sysmon64

# Verify events in Event Viewer
# Navigate to: Applications and Services Logs → Microsoft → Windows → Sysmon → Operational
```

---

## Step 7: Configure Wazuh for Windows Log Collection

The Wazuh agent collects Windows Event Logs by default. Ensure Sysmon and critical Windows logs are being forwarded.

### Verify Configuration

Check the Wazuh agent configuration file located at:
- `C:\Program Files\Wazuh\ossec.conf` or
- `C:\Program Files (x86)\ossec-agent\ossec.conf`

Ensure log collection is configured for:
- Sysmon Operational logs
- Windows Security logs
- Windows Application logs
- Windows System logs

After making configuration changes, restart the agent:

```powershell
Restart-Service -Name wazuh-agent
```

---

## Step 8: Generate Test Telemetry

Create security events to test your detection capabilities.

### Install Atomic Red Team

Atomic Red Team provides safe MITRE ATT&CK technique simulations:

```powershell
# Clone Atomic Red Team
git clone https://github.com/redcanaryco/atomic-red-team.git C:\tools\atomic

# Follow the project documentation to run specific atomic tests
```

### Generate Basic Test Events

Create simple test events:

```powershell
# Process creation event
powershell -c "Start-Process notepad"

# Script execution event
powershell -NoProfile -Command "Write-Output 'Test' > C:\temp\test.txt"

# Create test file
New-Item -Path C:\temp\test.txt -ItemType File -Force
```

### Optional: EICAR Test File

Drop an EICAR test file to generate antivirus detection events (safe malware test string).

---

## Step 9: Verify Agent and Logs in Wazuh Dashboard

### Check Agent Status

Navigate to **Dashboard → Agents**:
- Agent should be listed with status "Active"
- Click on the agent to view detailed information

### Review Security Events

Navigate to **Dashboards/Discover/Alerts**:
- You should see events from Sysmon
- Windows Event Log entries should be visible
- Search for specific event IDs or test activities

### Troubleshooting Connection Issues

If logs aren't appearing:

1. **Verify agent is running:**
   ```powershell
   Get-Service -Name wazuh-agent
   ```

2. **Check network connectivity** to Wazuh manager (default ports: 1514/1515)

3. **Configure Windows Firewall** if needed:
   ```powershell
   New-NetFirewallRule -DisplayName "Wazuh Agent Outbound" -Direction Outbound -LocalPort 1514 -Protocol TCP -Action Allow
   ```

4. **Review Docker logs** for server-side issues:
   ```bash
   docker-compose logs -f
   ```

---

## Step 10: Tune Detection Rules and Establish Baseline

### Install Rule Sets

Wazuh includes many default rule sets. Review and enable relevant ones for your environment.

### Reduce False Positives

Use the Wazuh dashboard to:
- Identify noisy rules in your lab environment
- Disable or adjust rules generating excessive alerts
- Create exceptions for known-good behavior

### Create Custom Rules

Develop custom detection rules for:
- Specific telemetry from Atomic Red Team tests
- Organizational security policies
- Lab-specific threat scenarios

---

## Common Troubleshooting Tips

| Issue | Solution |
|-------|----------|
| Docker containers fail to start | Increase Docker Desktop RAM/CPU allocation (Settings → Resources) |
| Agent can't connect to manager | Verify manager IP, check Windows Firewall, ensure port 1514/1515 is accessible |
| Sysmon events not appearing | Check Event Viewer: Applications and Services Logs → Microsoft → Windows → Sysmon → Operational |
| Container errors | Use `docker-compose logs -f` to view detailed error messages |
| WSL networking issues | Try using `host.docker.internal` as manager address instead of localhost |

---

## Next Steps

After completing this setup:

1. **Test thoroughly:** Run multiple Atomic Red Team tests and verify events appear in the dashboard
2. **Baseline your environment:** Document normal behavior to identify anomalies
3. **Integrate threat intelligence:** Consider MISP or OpenCTI integration for IOC management
4. **Develop playbooks:** Create incident response procedures based on Wazuh alerts
5. **Expand coverage:** Add more endpoints, configure custom rules, implement automated responses

---

## Additional Resources

- [Wazuh Documentation](https://documentation.wazuh.com/)
- [MITRE ATT&CK Framework](https://attack.mitre.org/)
- [Atomic Red Team](https://github.com/redcanaryco/atomic-red-team)
- [SwiftOnSecurity Sysmon Config](https://github.com/SwiftOnSecurity/sysmon-config)
- [Wazuh Rule Documentation](https://documentation.wazuh.com/current/user-manual/ruleset/index.html)

---

**Note:** This lab environment is designed for learning and testing purposes. Adjust security configurations appropriately before deploying in production environments.