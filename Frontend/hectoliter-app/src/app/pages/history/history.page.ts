import { Component, OnInit } from '@angular/core';
import { Router } from '@angular/router';

@Component({
  selector: 'app-history',
  templateUrl: './history.page.html',
  styleUrls: ['./history.page.scss'],
})
export class HistoryPage implements OnInit {

  constructor(private router: Router) { }

  ngOnInit() {
  }
  // cards = [
  //   { title: 'timestamp', content: '15/06/1997 - 15:00hrs' },
  //   { title: 'timestamp', content: '15/06/1997 - 15:00hrs' },
  //   { title: 'timestamp', content: '15/06/1997 - 15:00hrs' },
  //   { title: 'timestamp', content: '15/06/1997 - 15:00hrs' }
  // ];

  GoToMain() {
    this.router.navigate(['/main']);
  }
}
