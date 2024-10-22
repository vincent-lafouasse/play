import numpy as np
import matplotlib.pyplot as plt
import argparse


def main():
    parser = build_parser()
    args = parser.parse_args()
    indices, values = fetch_data(args.tsv_path)
    print(args)

    print(f"Plotting {args.tsv_path}")
    plot_data(indices, values, args)


def build_parser():
    parser = argparse.ArgumentParser(description="plot tsv to monitor your data")
    parser.add_argument(
        "tsv_path",
        metavar="tsv path",
        type=str,
        help="path to the tsv file you want to plot",
    )
    parser.add_argument("--xmin", help="set minimum x value", action="store")
    parser.add_argument("--xmax", help="set maximum x value", action="store")
    parser.add_argument("--ymin", help="set minimum y value", action="store")
    parser.add_argument("--ymax", help="set maximum y value", action="store")
    parser.add_argument(
        "--logx", help="set x scale to logarithmic", action="store_true"
    )
    parser.add_argument(
        "--logy", help="set y scale to logarithmic", action="store_true"
    )

    return parser


def fetch_data(tsv_path):
    with open(tsv_path, "r") as file:
        raw_data = file.read()

    lines = raw_data.strip("\n").split("\n")
    lines = [line.strip("\t").split("\t") for line in lines]
    data = []
    for line in lines:
        data.append([float(cell) for cell in line if cell])

    if len(lines) not in [1, 2]:
        print("Error: tsv format Y or X/Y")

    if len(lines) == 1:
        print("plotting input line as Y")
        values = np.array(data[0])
        indices = np.arange(0, len(values))
    else:
        print("plotting input lines as X Y")
        indices = np.array(data[0])
        values = np.array(data[1])

    return indices, values


def plot_data(indices, values, args):
    fig, ax = plt.subplots()

    if args.ymin is not None and args.ymax is not None:
        ymin = float(args.ymin)
        ymax = float(args.ymax)
        plt.ylim(bottom=ymin, top=ymax)
    elif args.ymin is not None:
        ymin = float(args.ymin)
        plt.ylim(bottom=ymin)
    elif args.ymax is not None:
        ymax = float(args.ymax)
        plt.ylim(top=ymax)

    if args.xmin is not None and args.xmax is not None:
        xmin = float(args.xmin)
        xmax = float(args.xmax)
        plt.xlim(left=xmin, right=xmax)
    elif args.xmin is not None:
        xmin = float(args.xmin)
        plt.xlim(left=xmin)
    elif args.xmax is not None:
        xmax = float(args.xmax)
        plt.xlim(right=xmax)

    if args.logx:
        ax.set_xscale("log")

    if args.logy:
        ax.set_yscale("log")

    plt.grid(visible=True, which="both", axis="both")
    ax.plot(indices, values)
    plt.show()


if __name__ == "__main__":
    main()
