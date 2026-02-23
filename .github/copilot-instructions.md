# Copilot Instructions for RawrXD Cloud Hosting

**Project**: RawrXD Cloud Hosting - Production deployment infrastructure for GGUF models
**Architecture**: DigitalOcean App Platform + Spaces (S3-compatible storage)
**Build**: Docker + Terraform for infrastructure as code
**Language**: Shell scripts, Terraform HCL, Docker, YAML

---

## 🏗 Architecture Overview

### Deployment Infrastructure

This repository implements a **production-ready cloud hosting solution** for deploying GGUF quantum models:

1. **Storage Layer** (DigitalOcean Spaces)
   - S3-compatible object storage with CDN
   - 250 GB included at $5/month
   - 1 TB free egress per month
   - Hosts 2GB quantum GGUF models

2. **Compute Layer** (DigitalOcean Droplets)
   - Basic Regular Droplet (1 vCPU / 1 GB RAM / 25 GB SSD)
   - $6/month cost
   - Runs llama.cpp server CPU-only
   - Handles light API traffic (10-50 req/s)

3. **CI/CD Layer** (GitHub Actions)
   - Automated Docker image builds
   - Push to GitHub Container Registry
   - Deploy to DigitalOcean App Platform
   - Triggers on push to main branch

---

## 🔑 Critical Patterns & Conventions

### 1. Deployment Scripts (Shell Scripts)

All deployment operations use bash scripts with error handling:

```bash
set -e  # Exit on error

# Configuration validation
if [ -z "${DROPLET_IP}" ]; then
    echo "Usage: $0 <droplet-ip>"
    exit 1
fi
```

**Why**: Ensures predictable behavior; failures are caught early.

### 2. Docker Multi-Stage Builds

Dockerfiles use multi-stage builds for minimal image size:

```dockerfile
FROM ubuntu:22.04 as builder
# ... build dependencies ...

FROM ubuntu:22.04
# ... runtime only ...
COPY --from=builder /build/llama.cpp/build/bin/server /usr/local/bin/llama-server
```

**Why**: Reduces image size (no build tools in production); faster deployments.

### 3. Infrastructure as Code (Terraform)

All infrastructure is defined in Terraform:

```hcl
resource "digitalocean_droplet" "llama_server" {
  name   = "rawrxd-llama-server"
  region = var.region
  size   = "s-1vcpu-1gb"
}
```

**Why**: Reproducible infrastructure; version-controlled changes.

### 4. Environment Variables

All sensitive configuration uses environment variables:

```yaml
envs:
  - key: DIGITALOCEAN_TOKEN
    value: ${{ secrets.DIGITALOCEAN_ACCESS_TOKEN }}
```

**Why**: Never commit secrets; supports multiple environments.

---

## 🛠 Build & Deployment

### Deployment Options

```bash
# Option 1: DigitalOcean App Platform (Recommended)
# - Push to main branch triggers automatic deployment
# - GitHub Actions builds Docker image
# - App Platform pulls and deploys

# Option 2: Terraform
cd deploy/terraform
terraform init
terraform apply

# Option 3: Manual SSH
bash deploy/scripts/deploy-to-digitalocean.sh <droplet-ip>
```

### Local Development

```bash
# Test locally before deploying
cd deploy
docker-compose up -d

# Test API
curl http://localhost:8080/health

# View logs
docker-compose logs -f

# Stop
docker-compose down
```

### Common Deployment Issues

| Issue | Root Cause | Fix |
|-------|-----------|-----|
| Docker build fails | Missing dependencies | Check Dockerfile layer-by-layer |
| App Platform deploy fails | Invalid app.yaml | Validate YAML syntax |
| Health check fails | Wrong port or path | Check EXPOSE directive and health_check config |
| Models not loading | Spaces not configured | Verify DIGITALOCEAN_TOKEN secret |

---

## 📂 File Organization

```
/
  .do/
    app.yaml              # DigitalOcean App Platform spec
  .github/
    workflows/
      deploy-digitalocean.yml  # CI/CD for automated deployment
      ci.yml                   # Build and test workflow
      build.yml                # Multi-platform build verification
    copilot-instructions.md    # This file
  deploy/
    docker/
      Dockerfile               # Multi-stage llama.cpp server build
      docker-compose.yml       # Local development setup
    scripts/
      deploy-to-digitalocean.sh   # Manual SSH deployment
      local-dev-setup.sh          # Local testing setup
      upload-to-spaces.sh         # Model upload to Spaces
    terraform/
      main.tf                  # Infrastructure definitions
      variables.tf             # Terraform variables
      user_data.sh             # Droplet initialization script
    README-CLOUD-HOSTING.md    # Detailed deployment guide
  README.md                    # Quick start guide
  setup.sh                     # Initial repository setup
  .gitignore                   # Excludes models, cache, secrets
```

---

## 🔴 Known Constraints & Gotchas

1. **DigitalOcean App Platform Limits**: App Platform expects specific directory structure; Dockerfile must be in deploy/docker/
2. **GitHub Container Registry**: Requires GITHUB_TOKEN with write:packages permission (auto-provided in Actions)
3. **Model Size Limits**: 2GB models fit in 1GB RAM droplets using streaming; larger models need 32GB+ droplets
4. **Spaces CDN**: CDN takes 10-15 minutes to propagate globally after first upload
5. **Health Checks**: llama.cpp server takes 30-40 seconds to start; configure health check accordingly

---

## 🎯 Common Tasks for AI Agents

### Task 1: Add a New Deployment Target
1. Create new Terraform resource in `deploy/terraform/main.tf`
2. Add variables in `variables.tf` 
3. Update `.do/app.yaml` if using App Platform
4. Test locally with `terraform plan`
5. Update documentation in `README-CLOUD-HOSTING.md`

### Task 2: Debug Deployment Failures
1. Check GitHub Actions logs for build/push errors
2. Verify DigitalOcean App Platform logs: `doctl apps logs <app-id>`
3. Check health endpoint: `curl http://<droplet-ip>:8080/health`
4. Review Docker logs: `docker logs llama-cpp-server`

### Task 3: Add New Model to Spaces
1. Place model file in `deploy/models/` directory
2. Run `bash deploy/scripts/upload-to-spaces.sh`
3. Update model list in documentation
4. Verify CDN URL: `https://rawrxd-quantum-models.nyc3.cdn.digitaloceanspaces.com/<model-name>`

---

## 📚 Key Documentation Files

- `README.md` — Quick start guide for deployment
- `deploy/README-CLOUD-HOSTING.md` — Detailed deployment guide with all options
- `.do/app.yaml` — DigitalOcean App Platform configuration spec
- `.github/workflows/deploy-digitalocean.yml` — CI/CD pipeline configuration
- `deploy/docker/Dockerfile` — Docker image build instructions
- `deploy/terraform/main.tf` — Infrastructure as code definitions

---

## ✅ Production Readiness Checklist

- **Deployment Options**: ✅ App Platform, Terraform, Manual SSH
  - ✅ App Platform auto-detects `.do/app.yaml`
  - ✅ Terraform provisions Droplets + Spaces
  - ✅ Manual scripts for SSH deployment
- **CI/CD**: ✅ GitHub Actions workflows
  - ✅ Automated Docker builds on push
  - ✅ Push to GitHub Container Registry
  - ✅ Deploy to DigitalOcean App Platform
- **Infrastructure**: ✅ Complete
  - ✅ Dockerfile with multi-stage builds
  - ✅ Docker Compose for local testing
  - ✅ Terraform for IaC
  - ✅ Health checks configured
- **Documentation**: ✅ Comprehensive
  - ✅ Quick start guide
  - ✅ Detailed deployment guide
  - ✅ Architecture documentation
  - ✅ Cost breakdown
- **Scripts**: ✅ All executable
  - ✅ deploy-to-digitalocean.sh
  - ✅ local-dev-setup.sh
  - ✅ upload-to-spaces.sh
- **Status**: 🚀 Production Ready
