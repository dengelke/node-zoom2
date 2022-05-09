import { Record } from './record';

export class Records {
  /** Tells whether there are remaining records to iterate over. */
  hasNext(): boolean;

  /** Returns next record in the iteration. */
  next(): Record;
}
