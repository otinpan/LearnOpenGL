# Texture
Texture��UV���W�ŊǗ������
![](https://learnopengl.com/img/getting-started/tex_coords.png)
���̂悤��UV���W�ł͍�����(0,0)�A�E�オ(1,1)�ɑΉ�����B
�摜�̓s�N�Z�����ƂɐF�����܂��Ă���B����ɑ΂��āAUV���W�͘A���I�ł���B
���̂��߁A����UV���W (x,y)�͂���s�N�Z���̂ǂ����Ɉʒu����B
�ł͂ǂ̂悤�� (x,y) ���_�̐F�����߂邩�B

## Filtering
�摜�̓s�N�Z�����ƂɐF�����܂��Ă���B����ɑ΂��āAUV���W�͘A���I�ł���B
���̂��߁A����UV���W (x,y)�͂���s�N�Z���̂ǂ����Ɉʒu����B
�ł͂ǂ̂悤�� (x,y) ���_�̐F�����߂邩�B1��**�ŋߖT�⊮** (nearest neighor)�A
����1��**���`�⊮** (liner interpolation) �ł���B
* nearest neighor�ł́A(x,y) ��̃s�N�Z���̐F�����̂܂ܓK�p����
* liner interpolation�ł� (x,y) ����̃s�N�Z�����狗���Ƃ̊����ŐF�����肷��

GL_NEAREST_
![](https://learnopengl.com/img/getting-started/filter_nearest.png)
![](https://learnopengl.com/img/getting-started/texture_filtering.png)

GL_LINER
![](https://learnopengl.com/img/getting-started/filter_linear.png)
![](https://learnopengl.com/img/getting-started/texture_filtering.png)

### Mipmaps
�I�u�W�F�N�g����R����A�����̂��̂Ƌ߂��̂��̂�����Ƃ���B���̂܂܂ł́A�����̂��̂Ƌ߂��̂��̂�
�����𑜓x (�Ⴆ�� 512x512) �̃e�N�X�`����K�p����B�����̕������𑜓x�̃e�N�X�`���ŕ\�����悤�Ƃ���ƁA
�����̃s�N�Z������A�L���͈͂��\����1�F��I�ԕK�v������B���̌��ʁA�����ȃI�u�W�F�N�g�ɂ̓A�[�e�B�t�@�N�g
�@(�s���R�ȕ\��) ���������邾���łȂ��A�������ш�̖��ʂɂ��Ȃ�B���̖����������邽�߂�**�~�b�v�}�b�v** 
(mipmaps) ������B�I�u�W�F�N�g������ȏ㗣�ꂽ�I�u�W�F�N�g�ɑ΂��ẮA�ł��K�����T�C�Y�̃~�b�v�}�b�v�摜
���g���B�I�u�W�F�N�g��������΁A��𑜓x�̃e�N�X�`���������B  
�~�b�v�}�b�v��؂�ւ���ہAOpenGL�ł̓~�b�v�}�b�v�̋��E���ڗ��悤�ȃA�[�e�B�t�@�N�g���\������邱�Ƃ�����B
����́A�قȂ�~�b�v�}�b�v�Ԃł̕⊮���s���Ă��Ȃ��ۂɔ�������B

##### `GL_NEAREST_MIPMAP_NEAREST`
�s�N�Z���T�C�Y�ɍł��߂��~�b�v�}�b�v���x����1�I�сA���̃��x������ŋߖT�⊮�ŐF�����
* �ł�����
* �������M�U�M�U�Ԃ��o�₷��
* �e�⃊�A���^�C���`��Ȃǂ̍����`�ʂɌ���

##### `GL_LINER_MIPMAP_NEAREST`
�ł��߂�1�̃~�b�v�}�b�v���x����I�сA���̃��x���̒��ŁA���`�⊮�ŐF�����
* �Ȃ߂炩�����ł�
* �~�b�v�}�b�v�Ԃ̋��E���ڗ����Ƃ�����

##### `GL_NEAREST_MIPMAP_LINER`
�s�N�Z���T�C�Y�ɍ���������2�̃~�b�v�}�b�v���x����I�ԁB���̊Ԃ���`�⊮���Ē��ԃ��x���̒l���o���B
�e���x���ł͍ŋߖ_�⊮�ŐF�����
* �~�b�v�}�b�v�Ԃ̂Ȃ��ڂ͊��炩
* �e�N�Z�� (�e�N�X�`���̃s�N�Z��) �̐��x�͂��e��

##### `GL_LINER_MIPMAP_LINER`
2�̃~�b�v�}�b�v���x����I�сA�e���x���Ő��`�⊮���s��B���̌��ʂ��X�ɐ��`�⊮���Ċ��炩�ɂ���
* �ł����炩�ō��i��
* �����R�X�g������

```cpp
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
```

### Loading and Creating textures
�l�X�ȃt�H�[�}�b�g�̉摜�t�@�C����Ǝ��Ń��[�h����̂͑�ρB������std_image.h���g���B
```cpp
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
```

�摜�̃��[�h�ɂ�`std_load()`���g��
```cpp
int width, height, nrChannels;
unsigned char *data = stbi_load("container.jpg", &width, &height, &nrChannels, 0); 
```
�ł͎��ۂ�texture�����
```cpp
unsigned int texture;
glGenTextures(1, &texture);  
```
Texture��ID��`texture`�ɕۑ�����

```cpp
glBindTexture(GL_TEXTURE_2D,texture);
```
�Ő������ꂽID���o�C���h���ăA�N�e�B�u�ɂ���
```cpp
glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
glGenerateMipmap(GL_TEXTURE_2D);
```
* ��1�����ł�`GL_TEXTURE_2D`���w�肷��ƁA���݃o�C���h����Ă���2D�e�N�X�`���I�u�W�F�N�g��
�΂��đ��삪�s����
* ��2�����ł̓~�b�v�}�b�v���x�����w�肷��B����͊�{���x����0
* ��3�����ł́AOpenGL�Ƀe�N�X�`�����ǂ̃t�H�[�}�b�g�ŕۑ����邩��`����B�����RGB�l�̂ݎ����Ă�
�邽�߁A�e�N�X�`����RGB�ŕۑ�����
* ��4�A�T�����ł͍����ƕ���ݒ肷��
* ��6������0�ɂ���
* ��7�C8�����͌��̉摜�f�[�^�̃t�H�[�}�b�g�ƃf�[�^�����w�肷��B����͉摜��RGB�t�H�[�}�b�g�œǂݍ��݁A
char�^�ŕێ����Ă��邽�߁A���ꂼ��ɑΉ�����l��n��

![container](Assets/container.jpg)










