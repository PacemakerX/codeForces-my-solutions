import requests
import os

def fetch_accepted_solutions(handle, output_dir="accepted_solutions"):
    # Base API URL
    submissions_url = f"https://codeforces.com/api/user.status?handle={handle}&from=1&count=1000"

    try:
        # Fetch submissions from the Codeforces API
        response = requests.get(submissions_url)
        response.raise_for_status()
        data = response.json()

        if data["status"] != "OK":
            print("Failed to fetch submissions!")
            return

        # Process submissions
        submissions = data["result"]
        submissions = sorted(submissions,key=lambda x: x['id'],reverse=True)
        accepted_solutions = []
        total_accepted = 0

        # Create output directory if it doesn't exist
        if not os.path.exists(output_dir):
            os.makedirs(output_dir)

        for submission in submissions:
            if submission["verdict"] == "OK":
                total_accepted += 1
                submission_id = submission["id"]
                contest_id = submission["contestId"]
                problem_index = submission["problem"]["index"]

                # Generate the file name
                file_name = f"{submission_id}_{contest_id}_{problem_index}.cpp"
                file_path = os.path.join(output_dir, file_name)

                # Create an empty .cpp file
                with open(file_path, "w", encoding="utf-8") as file:
                    file.write("")  # No content written inside the file

                accepted_solutions.append(file_name)

        # Print summary
        print("\nAccepted Solutions Saved:")
        for solution in sorted(accepted_solutions):
            print(solution)
        print(f"\nTotal Accepted Solutions: {total_accepted}")

    except requests.exceptions.RequestException as e:
        print(f"Error fetching submissions: {e}")

if __name__ == "__main__":
    # Replace with your Codeforces handle

    fetch_accepted_solutions("PacemakerX")
