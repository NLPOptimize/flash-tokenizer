# Build method for generating whl files per OS


<p>
<img align="left" src="https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white">
<img align="left" src="https://img.shields.io/badge/python-3670A0?style=for-the-badge&logo=python&logoColor=ffdd54">
<img align="left" src="https://img.shields.io/badge/Anaconda-%2344A833.svg?style=for-the-badge&logo=anaconda&logoColor=white">
</p><br>


## ✅ ManyLinux(x86_64)

<p>
<img align="left" src="https://img.shields.io/badge/Ubuntu-E95420?style=for-the-badge&logo=ubuntu&logoColor=white">
<img align="left" src="https://img.shields.io/badge/-Rocky%20Linux-%2310B981?style=for-the-badge&logo=rockylinux&logoColor=white">
<img align="left" src="https://img.shields.io/badge/Arch%20Linux-1793D1?logo=arch-linux&logoColor=fff&style=for-the-badge">
<img align="left" src="https://img.shields.io/badge/cent%20os-002260?style=for-the-badge&logo=centos&logoColor=F0F0F0">
<img align="left" src="https://img.shields.io/badge/Debian-D70A53?style=for-the-badge&logo=debian&logoColor=white">
<img align="left" src="https://img.shields.io/badge/Fedora-294172?style=for-the-badge&logo=fedora&logoColor=white">
<img align="left" src="https://img.shields.io/badge/openSUSE-%2364B345?style=for-the-badge&logo=openSUSE&logoColor=white">
<img align="left" src="https://img.shields.io/badge/Red%20Hat-EE0000?style=for-the-badge&logo=redhat&logoColor=white">
<img align="left" src="https://img.shields.io/badge/Ubuntu%20MATE-84A454.svg?style=for-the-badge&logo=Ubuntu-MATE&logoColor=white">
</p><br>



In the case of this project, you need to enter Docker and build it yourself.

#### 01. Run the Docker container with the following command in the flash-tokenizer/prj directory.
```bash
cd flash-tokenizer/prj
docker pull quay.io/pypa/manylinux_2_28
docker run -it -v %cd%:/io quay.io/pypa/manylinux_2_28 /bin/bash
```

#### 02. Execute the following command in Docker

```bash
/opt/python/cp313-cp313/bin/pip install build scikit-build-core pybind11 && \
/opt/python/cp312-cp312/bin/pip install build scikit-build-core pybind11 && \
/opt/python/cp311-cp311/bin/pip install build scikit-build-core pybind11 && \
/opt/python/cp310-cp310/bin/pip install build scikit-build-core pybind11 && \
/opt/python/cp39-cp39/bin/pip install build scikit-build-core pybind11 && \
/opt/python/cp38-cp38/bin/pip install build scikit-build-core pybind11

cd /io

CC=gcc CXX=g++ /opt/python/cp313-cp313/bin/python -m build --wheel && \
CC=gcc CXX=g++ /opt/python/cp312-cp312/bin/python -m build --wheel && \
CC=gcc CXX=g++ /opt/python/cp311-cp311/bin/python -m build --wheel && \
CC=gcc CXX=g++ /opt/python/cp310-cp310/bin/python -m build --wheel && \
CC=gcc CXX=g++ /opt/python/cp39-cp39/bin/python -m build --wheel && \
CC=gcc CXX=g++ /opt/python/cp38-cp38/bin/python -m build --wheel

mkdir wheelhouse && \
auditwheel repair dist/*.whl --wheel-dir /io/wheelhouse && \
rm -f dist/*linux*.whl && \
mv wheelhouse/*.whl dist/ && \
rm -rf wheelhouse
```



## ✅ Windows10/11, Server2022(amd64)


<p>
<img align="left" src="https://img.shields.io/badge/Windows-0078D6?style=for-the-badge&logo=windows&logoColor=white">
<img align="left" src="https://img.shields.io/badge/Windows%2011-%230079d5.svg?style=for-the-badge&logo=Windows%2011&logoColor=white">
<img align="left" src="https://img.shields.io/badge/Windows_Server_2022-0078D6?style=for-the-badge&logo=windows&logoColor=white">
</p><br>


#### 01. Install miniconda3
```bash
curl https://repo.anaconda.com/miniconda/Miniconda3-latest-Windows-x86_64.exe -o .\miniconda.exe
start /wait "" .\miniconda.exe /S
del .\miniconda.exe
```

#### 02. Configure the Python environment for each version and install the necessary libraries.
```bash
conda create -n python39 python=3.9 -y && ^
conda create -n python310 python=3.10 -y && ^
conda create -n python311 python=3.11 -y && ^
conda create -n python312 python=3.12 -y && ^
conda create -n python313 python=3.13 -y 

conda activate python39 && pip install build twine pybind11 numpy setuptools wheel --no-warn-script-location transformers && ^
conda activate python310 && pip install build twine pybind11 numpy setuptools wheel --no-warn-script-location transformers && ^
conda activate python311 && pip install build twine pybind11 numpy setuptools wheel --no-warn-script-location transformers && ^
conda activate python312 && pip install build twine pybind11 numpy setuptools wheel --no-warn-script-location transformers && ^
conda activate python313 && pip install build twine pybind11 numpy setuptools wheel --no-warn-script-location transformers
```

#### 03. Create a whl file for each version

```bash
conda activate python39 && python -m build --wheel && ^
conda activate python310 && python -m build --wheel && ^
conda activate python311 && python -m build --wheel && ^
conda activate python312 && python -m build --wheel && ^
conda activate python313 && python -m build --wheel
```

## ✅ macOS(arm64)

<p>
<img align="left" src="https://img.shields.io/badge/mac%20os(arm64)-000000?style=for-the-badge&logo=macos&logoColor=F0F0F0">
</p><br>


#### 01. Creating a Python environment from Python 3.8 to 3.14
```bash

conda create -n python39 python=3.9 -y &&\
conda create -n python310 python=3.10 -y &&\
conda create -n python311 python=3.11 -y &&\
conda create -n python312 python=3.12 -y &&\
conda create -n python313 python=3.13 -y &&\
conda create -n python314 python=3.14 -y

conda activate python39 && conda install build twine pybind11 numpy setuptools wheel -y &&\
conda activate python310 && conda install build twine pybind11 numpy setuptools wheel -y &&\
conda activate python311 && conda install build twine pybind11 numpy setuptools wheel -y &&\
conda activate python312 && pip install build twine pybind11 numpy setuptools wheel -y &&\
conda activate python313 && conda install build twine pybind11 numpy setuptools wheel -y &&\
conda activate python314 && conda install build twine pybind11 numpy setuptools wheel -y
```

```bash
uv venv .venv39 && source .venv39/bin/activate && uv pip install build twine pybind11 numpy setuptools wheel &&
uv venv .venv310 && source .venv310/bin/activate && uv pip install build twine pybind11 numpy setuptools wheel  &&
uv venv .venv311 && source .venv311/bin/activate && uv pip install build twine pybind11 numpy setuptools wheel &&
uv venv .venv312 && source .venv312/bin/activate && uv pip install build twine pybind11 numpy setuptools wheel &&
uv venv .venv313 && source .venv313/bin/activate && uv pip install build twine pybind11 numpy setuptools wheel  &&
uv venv .venv314 && source .venv314/bin/activate && uv pip install build twine pybind11 numpy setuptools wheel 
```


#### 02.
```bash

conda activate python39 && python -m build --wheel && \
conda activate python310 && python -m build --wheel && \
conda activate python311 && python -m build --wheel && \
conda activate python312 && python -m build  --wheel && \
conda activate python313 && python -m build --wheel && \
conda activate python314 && python -m build --wheel
```

