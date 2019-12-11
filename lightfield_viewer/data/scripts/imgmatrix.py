#! /usr/bin/env python

import argparse
import itertools

import matplotlib.pyplot as plt
import numpy as np

if __name__=='__main__':
    parser = argparse.ArgumentParser(
        description='Arrange a number of images as a matrix.')
    parser.add_argument('f', help='Output filename.')
    parser.add_argument('w', type=int,
                        help='Width of the matrix (number of images).')
    parser.add_argument('h', type=int,
                        help='Height of the matrix (number of images).')
    parser.add_argument('img', nargs='+', help='Images (w x h files).')
    parser.add_argument('--margin', metavar='m', nargs=1,
                        help='Margin between images: integers are '
                             'interpreted as pixels, floats as proportions.')
    args = parser.parse_args()

    w = args.w
    h = args.h
    n = w*h

    if len(args.img) != n:
        raise ValueError('Number of images ({}) does not match '
                         'matrix size {}x{}'.format(w, h, len(args.img)))

    imgs = [plt.imread(i) for i in args.img]

    if any(i.shape[0:1] != imgs[0].shape[0:1] for i in imgs[1:]):
        raise ValueError('Not all images have the same shape.')

    img_h, img_w = imgs[0].shape[0:2]
    img_c = 4 # RGBA

    m_x = 0
    m_y = 0
    if args.margin is not None:
        margin = args.margin[0]
        if '.' in margin:
            m = float(margin)
            m_x = int(m*img_w)
            m_y = int(m*img_h)
        else:
            m_x = int(margin)
            m_y = m_x

    imgmatrix = np.zeros((img_h * h + m_y * (h - 1),
                          img_w * w + m_x * (w - 1),
                          img_c))

    imgmatrix.fill(0.0)

    positions = itertools.product(range(w), range(h))
    for (x_i, y_i), img in zip(positions, imgs):
        x = x_i * (img_w + m_x)
        y = y_i * (img_h + m_y)
        if(img.shape[2] == 3): #rgb
            imgmatrix[y:y+img_h, x:x+img_w, 0:3] = img
            imgmatrix[y:y+img_h, x:x+img_w, 3] = 1.0
        elif(img.shape[2] == 4):
            imgmatrix[y:y+img_h, x:x+img_w, :] = img
        else:
            raise ValueError('Image must have 3 or 4 dimensions (rgb/rgba).')


    ticks = [256+512*i for i in range(0,w,1)]
    labels = [str(i) for i in [0.001, 0.005, 0.01, 0.05, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7]]
    
    fig = plt.figure(figsize=(w,w), dpi= 512)
    ax1 = fig.add_subplot(111)
    ax1.set_title('Approximation Anisotropic two bounces')
    ax1.set_xlabel(r'$\alpha_x$')
    ax1.set_ylabel(r'$\alpha_y$')

    ax1.set_xticks(ticks)
    ax1.set_yticks(ticks)

    ax1.set_xticklabels(labels)
    labels.reverse()
    ax1.set_yticklabels(labels)
    ax1.imshow(imgmatrix)

    fig.savefig(args.f)
