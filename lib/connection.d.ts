import { Readable } from 'stream';
import { Records } from './records';

declare enum QueryTypes {
  prefix, // PQF (Prefix Query Format)
  ccl, // CCL (Common Command Language)
  ccl2rpn, // CCL converted to RPN
  cql, // CQL (Common Query Language)
  cql2rpn, // CQL converted to RPN
}

export type QueryType = keyof typeof QueryTypes;

export class Connection extends Readable {
  /** Sets specified connection property. */
  set(key: string, val: string): this;

  /** Returns specified connection property. */
  get(key: string): string;

  /** Performs specified query. */
  query(type: QueryType, queryString: string): this;

  /** Performs specified query. */
  query(queryString: string): this;

  /** Updates specified record. */
  updateRecord(
    options: { [index: string]: string },
    cb: (error: Error, records: Records) => void
  ): void;

  /**
   * @param criteria String like "field1 flags1 field2 flags2 ..." specifying
   * what/how sorting should be applied.
   * Fields are either like "type=value" separated by comma (e.g. "1=4,2=1") or
   * plain string criteria (e.g. "title").
   * Flags are composed of a sequence of the following characters which may not
   * be separated by any white space.

   * Sort Flags:
   * - "a": ascending
   * - "d": descending
   * - "i": case insensitive
   * - "s": case sensitive
   */
  sort(criteria: string): this;

  /** Creates a read stream from this connection. */
  createReadStream(): Readable;
}
