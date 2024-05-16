import { NgModule } from '@angular/core';
import { Routes, RouterModule } from '@angular/router';

import { EndErrorPage } from './end-error.page';

const routes: Routes = [
  {
    path: '',
    component: EndErrorPage
  }
];

@NgModule({
  imports: [RouterModule.forChild(routes)],
  exports: [RouterModule],
})
export class EndErrorPageRoutingModule {}
