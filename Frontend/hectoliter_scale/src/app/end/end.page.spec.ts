import { ComponentFixture, TestBed } from '@angular/core/testing';
import { EndPage } from './end.page';

describe('EndPage', () => {
  let component: EndPage;
  let fixture: ComponentFixture<EndPage>;

  beforeEach(() => {
    fixture = TestBed.createComponent(EndPage);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
