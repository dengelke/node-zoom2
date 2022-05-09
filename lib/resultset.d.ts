import { Records } from './records';

export class ResultSet {
  /** Sets specified result set property. */
  set(key: string, val: string): this;

  /** Returns specified result set property. */
  get(key: string): string;

  size: number;

  /** Destroy this result set. */
  destroy(): void;

  /** Fetch records in this result set. */
  getRecords(
    index: number,
    counts: number,
    /** Callback function called after records were fetched. */
    cb: (error: Error, records: Records) => void
  ): void;
}
