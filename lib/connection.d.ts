import { Readable } from 'stream';
import { Records } from './records';
import { ResultSet } from './resultset';

declare enum QueryTypes {
  prefix, // PQF (Prefix Query Format)
  ccl, // CCL (Common Command Language)
  ccl2rpn, // CCL converted to RPN
  cql, // CQL (Common Query Language)
  cql2rpn, // CQL converted to RPN
}

export type QueryType = keyof typeof QueryTypes;

export class Connection extends Readable {
  /**
   * Creates a connection to the specified host.
   *
   * Note: user & password can be set using `#set` method like:
   * ```
   * connection
   *   .set('user', 'johnny')
   *   .set('password', 'ca$h')
   * ```
   *
   * @param host Host to connect to, like "z3950.loc.gov:7090".
   */
  constructor(host: string);

  /** Sets specified connection property. */
  set(key: string, val: string): this;

  /** Returns specified connection property. */
  get(key: string): string;

  /**
   * Establishes a connection.
   *
   * Note: returned value is actually a clone of the connection.
   */
  connect(cb: (error: Error) => void): this;

  /**
   * Prepares search query using specified query type.
   *
   * Note: returned value is actually a clone of the connection.
   */
  query(type: QueryType, queryString: string): this;

  /**
   * Prepares search query.
   *
   * Note: returned value is actually a clone of the connection.
   */
  query(queryString: string): this;

  /**
   * Searches for records using previously specified query.
   *
   * @see #query
   */
  search(cb: (error: Error, results: ResultSet) => void): this;

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
