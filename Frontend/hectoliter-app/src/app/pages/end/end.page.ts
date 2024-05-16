import { Component, OnInit } from '@angular/core';
import { Router } from '@angular/router';

@Component({
  selector: 'app-end',
  templateUrl: './end.page.html',
  styleUrls: ['./end.page.scss'],
})
export class EndPage implements OnInit {

  constructor(private router: Router) { }

  ngOnInit() {
  }

  GoToMain() {
    this.router.navigate(['/main']);
  }

  GoToHistory() {
    this.router.navigate(['/main']);
  }

}
