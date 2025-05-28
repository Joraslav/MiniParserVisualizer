import sys
import json
import matplotlib.pyplot as plt

def main():
    try:
        data = json.load(sys.stdin)
        if "files" not in data:
            print("Invalid input JSON: missing 'files'", file=sys.stderr)
            sys.exit(1)

        fig, ax = plt.subplots()
        for file in data["files"]:
            filename = file["filename"]
            for point in file["points"]:
                group = int(point["group"])
                x = point["x"]
                y = point["y"]
                label = f"{filename}:{group}"
                ax.scatter(x, y, label=label)

        # убрать дубли в легенде
        handles, labels = plt.gca().get_legend_handles_labels()
        by_label = dict(zip(labels, handles))
        ax.legend(by_label.values(), by_label.keys(), fontsize='small')

        ax.set_title("Points by File and Group")
        ax.set_xlabel("X")
        ax.set_ylabel("Y")
        plt.savefig("output.png")
        print("Plot saved to output.png")

    except Exception as e:
        print(f"Error in plot.py: {e}", file=sys.stderr)
        sys.exit(1)

if __name__ == "__main__":
    main()
