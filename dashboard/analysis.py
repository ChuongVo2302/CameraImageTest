import pandas as pd
import matplotlib.pyplot as plt

def analyze_logs(csv_path="data/test_log.csv"):
    try:
        df = pd.read_csv(csv_path)
    except FileNotFoundError:
        print(f"File not found: {csv_path}. Run the C++ program first to generate logs.")
        return

    total_tested = len(df)
    passed_count = len(df[df['OverallStatus'] == 'PASS'])
    failed_count = len(df[df['OverallStatus'] == 'FAIL'])
    yield_rate = (passed_count / total_tested) * 100 if total_tested > 0 else 0

    print(f"Total Tested: {total_tested}")
    print(f"Passed: {passed_count}")
    print(f"Failed: {failed_count}")
    print(f"First-Pass Yield: {yield_rate:.2f}%")

    failures = df[df['OverallStatus'] == 'FAIL']['FailureReason']
    failure_counts = {}
    for reason_str in failures.dropna():
        reasons = [r.strip() for r in reason_str.split(';') if r.strip()]
        for r in reasons:
            failure_counts[r] = failure_counts.get(r, 0) + 1

    if failure_counts:
        fail_df = pd.DataFrame(list(failure_counts.items()), columns=['DefectType', 'Count'])
        fail_df = fail_df.sort_values(by='Count', ascending=False)

        plt.figure(figsize=(8, 5))
        plt.bar(fail_df['DefectType'], fail_df['Count'], color='crimson')
        plt.title('Pareto Chart - Defect Distribution')
        plt.xlabel('Defect Category')
        plt.ylabel('Occurrences')
        plt.tight_layout()
        plt.show()

if __name__ == "__main__":
    analyze_logs()
